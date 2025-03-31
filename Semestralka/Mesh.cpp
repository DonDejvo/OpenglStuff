#include "Mesh.h"
#include <iostream>
#include "AssetManager.h"

void Mesh::initFromScene(const aiScene* scene, const std::string& path)
{
	mDrawCalls.resize(scene->mNumMeshes);
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
	mDrawCalls[index].baseVertex = numVertices;
	mDrawCalls[index].baseIndex = numIndices;
	mDrawCalls[index].numIndices = mesh->mNumFaces * 3;
	mDrawCalls[index].materialIndex = mesh->mMaterialIndex;

	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		const aiVector3D& position = mesh->mVertices[i];
		const aiVector3D& texCoord = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][i] : zero3D;
		const aiVector3D& normal = mesh->mNormals[i];

		Vertex v;
		v.position = glm::vec3(position.x, position.y, position.z);
		v.texCoord = glm::vec2(texCoord.x, texCoord.y);
		v.normal = glm::vec3(normal.x, normal.y, normal.z);

		mVertices.push_back(v);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		const aiFace& face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			mIndices.push_back(face.mIndices[j]);
		}
	}

	numVertices += mesh->mNumVertices;
	numIndices += mDrawCalls[index].numIndices;
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
		initFromScene(scene, path);

		init();
	}
	else {
		std::cout << "Mesh failed to load\n";
	}
}

void Mesh::init()
{
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(NUM_BUFFERS, mBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, mBuffers[BufferType::VERTEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), &mVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffers[BufferType::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mIndices.size(), &mIndices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::draw(DrawCallbacks* drawCallbacks) const
{
	glBindVertexArray(mVAO);

	for (unsigned int i = 0; i < mDrawCalls.size(); ++i) {
		const Material& material = mMaterials[mDrawCalls[i].materialIndex];

		if (drawCallbacks) {

			if (material.diffuseTexture) {
				material.diffuseTexture->bind(GL_TEXTURE0);
				drawCallbacks->supplyMaterial(material);
			}

			if (material.specularTexture) {
				material.specularTexture->bind(GL_TEXTURE1);
				drawCallbacks->enableSpecularTexture(true);
			}
			else {
				drawCallbacks->enableSpecularTexture(false);
			}
		}

		glDrawElementsBaseVertex(GL_TRIANGLES, mDrawCalls[i].numIndices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int) * mDrawCalls[i].baseIndex), mDrawCalls[i].baseVertex);
	}

	glBindVertexArray(0);
}

Quad::Quad() {
	mVertices = {
		{ {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {0.5f, 0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },
		{ {0.5f, -0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f} },
		{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f} }
	};
	mIndices = { 0, 1, 2, 0, 2, 3 };
	mDrawCalls = {
		{0, 0, 6, 0}
	};

	init();
}
