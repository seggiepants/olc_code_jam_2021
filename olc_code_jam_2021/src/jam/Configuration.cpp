#include <sys/types.h>
#include <sys/stat.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "Configuration.h"
#if defined(OS_WIN) && !defined(__MINGW32__)
#include <atlbase.h>
#include <atlstr.h>
#include <comutil.h>
#include <direct.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif

namespace jam
{

	void Configuration::CreatePathIfNotExist(std::string path)
	{

		std::string pathBuffer;
		size_t offset = 0;
		// EACCES = 13
		// EBADF = 9
		// EFAULT = 14
		// ELOOP = 114
		// ENAMETOOLONG = 38
		// ENOENT = 2
		
		while (offset != std::string::npos)
		{
			std::string folder = "";
			offset = path.find(PATH_SEP, offset + 1);
			if (offset == std::string::npos)
			{
				folder = path;
			}
			else
			{
				folder = path.substr(0, offset);
			}
			if (folder.length() == 0 || folder == "\\\\" || folder == "\\" || (folder.length() == 2 && folder[1] == ':'))
			{
				continue;
			}
			struct stat info;
			int ret = stat(folder.c_str(), &info);
			if ((ret == 0) || ((ret == -1) && (errno == ENOENT)))
			{
				//std::cerr << "Cannot access: " << folder << "(" << errno << ")" << std::endl;
			//}
			//else 
				if (!(info.st_mode & S_IFDIR))
				{
#ifdef OS_WIN
#ifdef __MINGW32__
					std::wstring wpath(path.begin(), path.begin() + offset);
					_wmkdir(wpath.c_str());
#else
					_mkdir(path.substr(0, offset).c_str());
#endif
#endif
#ifdef OS_UNIX
					mkdir(path.substr(0, offset).c_str()), 0777);
#endif
				}
			}
			else
			{
				std::cerr << "Cannot access: " << folder << "(" << errno << ")" << std::endl;
			}
		}
	}

	std::string Configuration::GetAppPath()
	{
#if defined(__MINGW32__)
		return std::string(".");
#elif defined(OS_WIN)
		wchar_t path[MAX_PATH] = { 0 };
		GetModuleFileNameW(NULL, path, MAX_PATH);
		std::wstring ws(path);
		return std::string(ws.begin(), ws.end());
#elif defined(OS_UNIX)
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return std::string(result, (count > 0) ? count : 0);
#elif defined(OS_UNKNOWN)
		return std::string(".");
#endif
	}

	std::string Configuration::GetDataPath()
	{
#if defined(OS_WIN) && !defined(__MINGW32__)
		TCHAR szPath[MAX_PATH];
		// Get path for each computer, non-user specific and non-roaming data.
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
		{
			// Append product-specific path
			std::ostringstream ss;
			ss << "\\" << game::COMPANY << "\\" << game::PRODUCT << "\\" << game::VERSION << "\\";
			std::string targetPath = ss.str();

			// Convert char* string to a wchar_t* string.
			size_t copiedChars = 0;
			TCHAR* buffer = new TCHAR[targetPath.size() + 1];
			mbstowcs_s(&copiedChars, buffer, targetPath.size(), targetPath.c_str(), _TRUNCATE);
			PathAppend(szPath, buffer);
			delete[] buffer;
			std::wstring wPath(szPath);
			std::string ret = std::string(wPath.begin(), wPath.end());
			Configuration::CreatePathIfNotExist(ret);
			return ret;
		}
		else
		{
			std::string appPath = Configuration::GetAppPath();
			if (appPath[appPath.size() -1] != '\\' && appPath[appPath.size() -1] != '/')
				appPath.push_back('\\');
			std::ostringstream ss;
			ss << appPath.c_str() << game::COMPANY << "\\" << game::PRODUCT << game::VERSION << "\\";
			return ss.str();
		}
#elif defined(OS_UNIX)
		std::ostringstream ss;
		ss << "~/.config/" << game::COMPANY << "/" << game::PRODUCT << "/" << game::VERSION << "/";
		return ss.str();

#elif defined(OS_UNKOWN) || defined(__MINGW32__)
		std::string appPath = Configuration::GetAppPath();
		if (appPath[appPath.size() - 1] != '\\' && appPath[appPath.size() - 1] != '/')
			appPath.push_back('/');
		std::ostringstream ss;
		ss << appPath.c_str() << game::COMPANY << "/" << game::PRODUCT << "/" << game::VERSION << "/";
		return std::string(ss.str());
#endif
		return "";
	}

	nlohmann::json Configuration::LoadJsonFile(std::string fileName)
	{				
		std::ostringstream buffer;
		try
		{
			std::ifstream file(fileName);
			if (file.is_open())
			{
				std::string line;
				while (std::getline(file, line))
				{
					buffer << line << std::endl;
				}
				file.close();
			}
		}
		catch (std::exception ex)
		{
			std::cerr << "Unable to read file " << fileName << std::endl << ex.what() << std::endl;
		}
		try
		{
			return nlohmann::json::parse(buffer.str());
		}
		catch (std::exception ex)
		{
			std::cerr << "Unable to parse file " << fileName << std::endl << ex.what() << std::endl;
			return nullptr;
		}
	}

	bool Configuration::SaveJsonFile(std::string fileName, nlohmann::json data)
	{
		try
		{
			std::ofstream file(fileName);
			if (file.is_open())
			{
				file << std::setw(4) << data << std::endl;
				file.close();
			}
			else
			{
				std::cerr << "File not open: " << fileName << std::endl;
				return false;
			}
		}
		catch (std::exception ex)
		{
			std::cerr << "Unable to write file " << fileName << std::endl << ex.what() << std::endl;
			return false;
		}
		return true;
	}

	std::string Configuration::PathJoin(std::string path, std::string filename)
	{
		std::ostringstream ss;
		ss << path << PATH_SEP << filename;
		return ss.str();
	}
}
