#include "Mesh.h"
#include <iostream>
#include "AssetManager.h"

Mesh::Mesh() {
	mGeometry = nullptr;
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	pitch = 0;
	yaw = 0;
}

void Mesh::initFromScene(const aiScene* scene, const std::string& path)
{
	mGeometry->drawCalls.resize(scene->mNumMeshes);
	mMaterials.resize(scene->mNumMaterials);

	int numVertices = 0;
	int numIndices = 0;

	for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
		initSingleMesh(scene->mMeshes[i], i, numVertices, numIndices);
	}

	std::string directory = path.substr(0, path.find_last_of('/'));

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
		initMaterial(scene->mMaterials[i], i, directory);
	}
}

void Mesh::initSingleMesh(aiMesh* mesh, unsigned int index, int& numVertices, int& numIndices)
{
	mGeometry->drawCalls[index].baseVertex = numVertices;
	mGeometry->drawCalls[index].baseIndex = numIndices;
	mGeometry->drawCalls[index].numIndices = mesh->mNumFaces * 3;
	mGeometry->drawCalls[index].materialIndex = mesh->mMaterialIndex;

	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		const aiVector3D& position = mesh->mVertices[i];
		const aiVector3D& texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : zero3D;
		const aiVector3D& normal = mesh->mNormals[i];

		Geometry::Vertex v;
		v.position = glm::vec3(position.x, position.y, position.z);
		v.texCoord = glm::vec2(texCoord.x, texCoord.y);
		v.normal = glm::vec3(normal.x, normal.y, normal.z);

		mGeometry->vertices.push_back(v);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		const aiFace& face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			mGeometry->indices.push_back(face.mIndices[j]);
		}
	}

	numVertices += mesh->mNumVertices;
	numIndices += mGeometry->drawCalls[index].numIndices;
}

void Mesh::initMaterial(aiMaterial* material, unsigned int index, const std::string& directory)
{
	// Load diffuse textures
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			std::string fileName(path.C_Str());
			Texture* tex = AssetManager::getTexture(directory + "/" + fileName);
			mMaterials[index].diffuseTexture = tex;
		}
	}

	// Load specular textures
	if (material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
		aiString path;
		if (material->GetTexture(aiTextureType_SPECULAR, 0, &path) == AI_SUCCESS) {
			std::string fileName(path.C_Str());
			std::cout << "Specular tex name: " << fileName << "\n";
			Texture* tex = AssetManager::getTexture(directory + "/" + fileName);
			mMaterials[index].specularTexture = tex;
		}
	}

	// Load colors
	
	aiColor4D color;
	if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &color) == AI_SUCCESS) {
		std::cout << "Ambient color: rgb(" << color.r << "," << color.g << "," << color.b << ")\n";
		mMaterials[index].ambientColor = glm::vec3(color.r, color.g, color.b);
	}
	if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS) {
		std::cout << "Diffuse color: rgb(" << color.r << "," << color.g << "," << color.b << ")\n";
		mMaterials[index].diffuseColor = glm::vec3(color.r, color.g, color.b);
	}
	if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &color) == AI_SUCCESS) {
		std::cout << "Specular color: rgb(" << color.r << "," << color.g << "," << color.b << ")\n";
		mMaterials[index].specularColor = glm::vec3(color.r, color.g, color.b);
	}

}

void Mesh::loadFromFile(const std::string& path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, MESH_IMPORT_FLAGS);

	if (scene) {
		mGeometry = new Geometry();

		initFromScene(scene, path);

		mGeometry->initBuffers();
	}
	else {
		std::cout << "Mesh failed to load\n";
	}
}

void Mesh::draw(DrawCallbacks* drawCallbacks) const
{
	glBindVertexArray(mGeometry->getVAO());

	for (unsigned int i = 0; i < mGeometry->drawCalls.size(); ++i) {
		const Material& material = mMaterials[mGeometry->drawCalls[i].materialIndex];

		if (drawCallbacks) {
			drawCallbacks->supplyMaterial(material);

			if (material.diffuseTexture) {
				material.diffuseTexture->bind(GL_TEXTURE0);
				drawCallbacks->enableDiffuseTexture(true);
			}
			else {
				drawCallbacks->enableDiffuseTexture(false);
			}

			if (material.specularTexture) {
				material.specularTexture->bind(GL_TEXTURE1);
				drawCallbacks->enableSpecularTexture(true);
			}
			else {
				drawCallbacks->enableSpecularTexture(false);
			}
		}

		glDrawElementsBaseVertex(GL_TRIANGLES, mGeometry->drawCalls[i].numIndices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * mGeometry->drawCalls[i].baseIndex), mGeometry->drawCalls[i].baseVertex);
	}

	glBindVertexArray(0);
}

void Mesh::setMaterial(unsigned int idx, const Material& mat)
{
	if (mMaterials.size() < idx) {
		mMaterials.resize(idx);
	}
	mMaterials.insert(mMaterials.begin() + idx, mat);
}

void Mesh::computeModelMatrix()
{
	mMatrix = glm::mat4(1.0f);
	mMatrix = glm::scale(mMatrix, scale);
	mMatrix = glm::rotate(mMatrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	mMatrix = glm::rotate(mMatrix, yaw, glm::vec3(0.0f, cos(pitch), sin(pitch)));
	mMatrix = glm::translate(glm::mat4(1.0f), position) * mMatrix;
}