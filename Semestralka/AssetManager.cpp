#include "AssetManager.h"

AssetManager* AssetManager::get()
{
	if (instance == nullptr) {
		instance = new AssetManager();
	}
	return instance;
}

Texture* AssetManager::getTexture(const std::string& path)
{
	if (textureMap.find(path) == textureMap.end()) {
		Texture* texture = new Texture();
		texture->loadFromFile(path);
		textureMap[path] = texture;
	}
	return textureMap[path];
}

Mesh::Data AssetManager::getMesh(const std::string& path)
{
	if (meshMap.find(path) == meshMap.end()) {
		Mesh::Data meshData = Mesh::loadDataFromFile(path);
		meshMap[path] = meshData;
	}
	return meshMap[path];
}

AssetManager* AssetManager::instance = nullptr;