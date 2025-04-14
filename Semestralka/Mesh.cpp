#include "Mesh.h"
#include <iostream>
#include "AssetManager.h"

Mesh::Mesh() {
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	pitch = 0;
	yaw = 0;
}

void Mesh::initFromScene(Data& data, const aiScene* scene, const std::string& path)
{
	data.geometry = new Geometry();

	data.geometry->drawCalls.resize(scene->mNumMeshes);
	data.materials.resize(scene->mNumMaterials);

	int numVertices = 0;
	int numIndices = 0;

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		initSingleMesh(data, scene->mMeshes[i], i, numVertices, numIndices);
	}

	std::string directory = path.substr(0, path.find_last_of('/'));

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
		initMaterial(data, scene->mMaterials[i], i, directory);
	}

	data.geometry->computeTangents();
	data.geometry->initBuffers();
}

void Mesh::initSingleMesh(Data& data, aiMesh* mesh, unsigned int index, int& numVertices, int& numIndices)
{
	data.geometry->drawCalls[index].baseVertex = numVertices;
	data.geometry->drawCalls[index].baseIndex = numIndices;
	data.geometry->drawCalls[index].numIndices = mesh->mNumFaces * 3;
	data.geometry->drawCalls[index].materialIndex = mesh->mMaterialIndex;

	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		const aiVector3D& position = mesh->mVertices[i];
		const aiVector3D& texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : zero3D;
		const aiVector3D& normal = mesh->mNormals[i];

		Geometry::Vertex v;
		v.position = glm::vec3(position.x, position.y, position.z);
		v.texCoord = glm::vec2(texCoord.x, texCoord.y);
		v.normal = glm::vec3(normal.x, normal.y, normal.z);

		data.geometry->vertices.push_back(v);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		const aiFace& face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			data.geometry->indices.push_back(face.mIndices[j]);
		}
	}

	numVertices += mesh->mNumVertices;
	numIndices += data.geometry->drawCalls[index].numIndices;
}

void Mesh::initMaterial(Data& data, aiMaterial* material, unsigned int index, const std::string& directory)
{
	data.materials[index] = new Material();

	// Load diffuse textures
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			std::string fileName(path.C_Str());
			Texture* tex = AssetManager::get()->getTexture(directory + "/" + fileName);
			data.materials[index]->diffuseTexture = tex;
		}
	}

	// Load specular textures
	if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
		aiString path;
		if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
			std::string fileName(path.C_Str());
			std::cout << "Specular tex name: " << fileName << "\n";
			Texture* tex = AssetManager::get()->getTexture(directory + "/" + fileName);
			data.materials[index]->specularTexture = tex;
		}
	}

	/*if (material->GetTextureCount(aiTextureType_HEIGHT) > 0) {
		aiString path;
		if (material->GetTexture(aiTextureType_HEIGHT, 0, &path) == AI_SUCCESS) {
			std::string fileName(path.C_Str());
			std::cout << "Normal tex name: " << fileName << "\n";
			Texture* tex = AssetManager::get()->getTexture(directory + "/" + fileName);
			data.materials[index]->normalMap = tex;
		}
	}*/

	// Load colors
	
	aiColor4D color;
	if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS) {
		std::cout << "Ambient color: rgb(" << color.r << "," << color.g << "," << color.b << ")\n";
		data.materials[index]->ambientColor = glm::vec3(color.r, color.g, color.b);
	}
	if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS) {
		std::cout << "Diffuse color: rgb(" << color.r << "," << color.g << "," << color.b << ")\n";
		data.materials[index]->diffuseColor = glm::vec3(color.r, color.g, color.b);
	}
	if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS) {
		std::cout << "Specular color: rgb(" << color.r << "," << color.g << "," << color.b << ")\n";
		data.materials[index]->specularColor = glm::vec3(color.r, color.g, color.b);
	}

}

Mesh::Data Mesh::loadDataFromFile(const std::string& path)
{
	Data data;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, MESH_IMPORT_FLAGS);

	if (scene) {
		initFromScene(data, scene, path);
	}
	else {
		std::cout << "Mesh failed to load\n";
	}

	return data;
}

void Mesh::loadFromFile(const std::string& path)
{
	mData = AssetManager::get()->getMesh(path);
}

void Mesh::draw(DrawCallbacks* drawCallbacks) const
{
	glBindVertexArray(mData.geometry->getVAO());

	for (unsigned int i = 0; i < mData.geometry->drawCalls.size(); ++i) {
		Material* material = mData.materials[mData.geometry->drawCalls[i].materialIndex];

		if (drawCallbacks) {
			drawCallbacks->supplyMaterial(*material);

			if (material->diffuseTexture) {
				material->diffuseTexture->bind(DIFFUSE);
				drawCallbacks->enableDiffuseTexture(true);
			}
			else {
				drawCallbacks->enableDiffuseTexture(false);
			}

			if (material->specularTexture) {
				material->specularTexture->bind(SPECULAR);
				drawCallbacks->enableSpecularTexture(true);
			}
			else {
				drawCallbacks->enableSpecularTexture(false);
			}

			if (material->normalMap) {
				material->normalMap->bind(NORMAL_MAP);
				drawCallbacks->enableNormalMap(true);
			}
			else {
				drawCallbacks->enableNormalMap(false);
			}
		}

		mData.geometry->draw(i);
	}

	glBindVertexArray(0);
}

void Mesh::setMaterial(unsigned int idx, Material* mat)
{
	if (mData.materials.size() < idx) {
		mData.materials.resize(idx);
	}
	mData.materials.insert(mData.materials.begin() + idx, mat);
}

void Mesh::computeModelMatrix()
{
	mMatrix = glm::mat4(1.0f);
	mMatrix = glm::scale(mMatrix, scale);
	mMatrix = glm::rotate(mMatrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	mMatrix = glm::rotate(mMatrix, yaw, glm::vec3(0.0f, cos(pitch), sin(pitch)));
	mMatrix = glm::translate(glm::mat4(1.0f), position) * mMatrix;
}