#pragma once

#include <map>
#include <string>
#include "Texture.h"

class AssetManager {
private:
	static AssetManager* instance;

	std::map<std::string, Texture*> textureMap;
public:
	static AssetManager* get() {
		if (instance == nullptr) {
			instance = new AssetManager();
		}
		return instance;
	}

	static Texture* getTexture(const std::string& path) {

		if (get()->textureMap.find(path) == get()->textureMap.end()) {
			Texture* texture = new Texture();
			texture->loadFromFile(path);
			get()->textureMap[path] = texture;
		}
		return get()->textureMap[path];
	}
};

AssetManager* AssetManager::instance = nullptr;