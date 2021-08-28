#pragma once
#ifndef __RESOURCE_MANAGER_PGE_H__
#define __RESOURCE_MANAGER_PGE_H__
#include <unordered_map>
#include <olcPixelGameEngine.h>
#include <olcPGEX_Sound.h>
#include "../../IFont.h"
#include "ImagePGE.h"
#include "../../IResourceManager.h"
#include "AudioPGE.h"

namespace jam
{
    class ResourceManagerPGE :
        public IResourceManager
    {
    public:
		~ResourceManagerPGE() override;
		/*
		int GetAudio(std::string path);
		olc::Font* GetFont(std::string path, int size);
		olc::Sprite* GetImage(std::string path);
		*/
		void Clear() override;
		IAudio* GetAudio(std::string) override;
		IFont* GetFont(std::string) override;
		IImage* GetImage(std::string) override;
		bool HasAudio(std::string) override;
		bool HasFont(std::string) override;
		bool HasImage(std::string) override;
		void PreloadAudio(std::string) override;
		void PreloadFont(std::string key, IFont* font) override;
		void PreloadImage(std::string) override;
		static IResourceManager* Instance() {
			if (!instance) {
				instance = new ResourceManagerPGE();
			}
			return instance;
		}
    protected:
		ResourceManagerPGE();
		static ResourceManagerPGE* instance;
		std::unordered_map<std::string, jam::AudioPGE*> audio;
		std::unordered_map<std::string, jam::ImagePGE*> image;
		std::unordered_map<std::string, jam::IFont*> font;

    };
}

#endif
