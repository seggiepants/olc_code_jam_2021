#include <iostream>
#include "BackendSDL2.h"
#include "ResourceManagerSDL2.h"
#include "../../Shared.h"

namespace jam
{
	ResourceManagerSDL2* ResourceManagerSDL2::instance = NULL;
	ResourceManagerSDL2::ResourceManagerSDL2() {}
	ResourceManagerSDL2::~ResourceManagerSDL2() 
	{
		this->Clear();
	}

	void ResourceManagerSDL2::Clear() 
	{
		if (instance != NULL)
		{
			for (const auto& element : instance->audio) {
				delete element.second;
			}
			instance->audio.clear();

			for (const auto& element : instance->font) {
				delete element.second;
			}
			instance->font.clear();

			for (const auto& element : instance->image) {
				delete element.second;
			}
			instance->image.clear();

			ResourceManagerSDL2* temp = instance;
			instance = NULL;
			delete temp;
		}
	}
	IAudio* ResourceManagerSDL2::GetAudio(std::string filePath) 
	{ 
		this->PreloadAudio(filePath);
		return this->audio[filePath];
	}

	IFont* ResourceManagerSDL2::GetFont(std::string key) 
	{
		return instance->font[key];
	}

	IImage* ResourceManagerSDL2::GetImage(std::string key) 
	{
		return  instance->image[key];
	}

	bool ResourceManagerSDL2::HasAudio(std::string filePath) 
	{ 
		std::unordered_map<std::string, AudioSDL2*>::const_iterator search = instance->audio.find(filePath);

		return (search != instance->audio.end());
	}

	bool ResourceManagerSDL2::HasFont(std::string key) 
	{ 
		std::unordered_map<std::string, jam::IFont*>::const_iterator search = instance->font.find(key);

		return (search != instance->font.end());
	}

	bool ResourceManagerSDL2::HasImage(std::string key) 
	{ 
		std::unordered_map<std::string, jam::ImageSDL2*>::const_iterator search = instance->image.find(key);

		return (search != instance->image.end());
	}
	
	void ResourceManagerSDL2::PreloadAudio(std::string filePath) 
	{
		if (!instance->HasAudio(filePath))
		{
			AudioSDL2* sample = new AudioSDL2();
			bool success = sample->Load(filePath);

			if (success)
			{
				instance->audio[filePath] = sample;
			}
		}
	}

	void ResourceManagerSDL2::PreloadFont(std::string key, IFont* font) 
	{
		if (!instance->HasFont(key))
		{
			if (font == nullptr)
			{
				std::cerr << "Invalid font for key \"" << key << "." << std::endl;
			}
			else
			{
				instance->font[key] = font;
			}
		}
	}

	void ResourceManagerSDL2::PreloadImage(std::string filePath) 
	{
		if (!instance->HasImage(filePath))
		{
			ImageSDL2* image = new ImageSDL2();
			bool success = image->Construct(filePath);

			if (success)
			{
				instance->image[filePath] = image;
			}
			else
			{
				std::cerr << "Error loading image \"" << filePath << "\"" << std::endl << SDL_GetError() << std::endl;
			}
		}
	}

}