#include "SceneManager.h"

namespace jam
{
	SceneManager* SceneManager::instance = nullptr;

	SceneManager::SceneManager()
	{
		// This should be redundant.
		this->scenes.clear();
	}

	SceneManager::~SceneManager()
	{
		for (const auto& element : this->scenes) {
			delete element.second;
		}
		this->scenes.clear();
	}

	void SceneManager::AddScene(std::string name, IScene* scene)
	{
		if (this->HasScene(name))
		{
			delete this->scenes[name];
		}
		this->scenes[name] = scene;
	}

	IScene* SceneManager::GetScene(std::string name)
	{
		if (this->HasScene(name))
		{
			return this->scenes[name];
		}
		else
		{
			return nullptr;
		}
	}

	bool SceneManager::HasScene(std::string name)
	{
		std::unordered_map<std::string, IScene*>::const_iterator search = instance->scenes.find(name);

		return (search != instance->scenes.end());
	}
}
