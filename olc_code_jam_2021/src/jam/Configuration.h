#pragma once
#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <string>
#include "../3rdParty/json/json.hpp"
#include "Shared.h"
#include "../game/Version.h"

namespace jam
{
	class Configuration
	{
	public:
		static void CreatePathIfNotExist(std::string path);
		static std::string GetAppPath();
		static std::string GetDataPath();
		static nlohmann::json LoadJsonFile(std::string fileName);
		static bool SaveJsonFile(std::string fileName, nlohmann::json data);
		static std::string PathJoin(std::string path, std::string filename);
	};
}
#endif