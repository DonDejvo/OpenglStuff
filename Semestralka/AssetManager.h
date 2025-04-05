#pragma once

#include <map>
#include <string>
#include "Texture.h"
#include "Mesh.h"

class AssetManager {
private:
	static AssetManager* instance;

	std::map<std::string, Texture*> textureMap;
	std::map<std::string, Mesh::Data> meshMap;
public:
	static AssetManager* get();

	Texture* getTexture(const std::string& path);

	Mesh::Data getMesh(const std::string& path);
};