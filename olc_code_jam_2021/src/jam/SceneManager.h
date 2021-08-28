#pragma once
#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__
#include <string>
#include <unordered_map>
#include "IScene.h"
namespace jam
{
	class SceneManager
	{
	public:
		~SceneManager();
		void AddScene(std::string name, IScene* scene);
		IScene* GetScene(std::string);
		bool HasScene(std::string);
		static SceneManager* Instance() {
			if (!instance) {
				instance = new SceneManager();
			}
			return instance;
		}
	protected:
		std::unordered_map<std::string, IScene*> scenes;
	private:
		SceneManager();
		static SceneManager* instance;
	};
}
#endif

