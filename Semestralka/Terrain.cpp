#include "Terrain.h"
#include "mathUtils.h"

TerrainManager::TerrainManager(const glm::vec2& dim, int terrainSize, TerrainMaterial* material)
    : dimensions(dim), terrainSize(terrainSize), material(material)
{
    for (int x = 0; x < dim.x; ++x) {
        for (int y = 0; y < dim.y; ++y) {
            std::string key = std::to_string(x) + "." + std::to_string(y);
            Terrain* terrain = new Terrain();
            terrain->x = x * terrainSize;
            terrain->z = y * terrainSize;
            terrain->size = terrainSize;
            terrain->setSegments(256, 256);
            terrain->init();
            terrain->setMaterial(material);
            mTerrains[key] = terrain;

        }
    }
}

float TerrainManager::getHeightAtPosition(const glm::vec3& position)
{
    int x = clamp((int)(position.x / terrainSize), 0, dimensions.x);
    int y = clamp((int)(position.z / terrainSize), 0, dimensions.y);

    std::string key = std::to_string(x) + "." + std::to_string(y);
    if (mTerrains.find(key) != mTerrains.end()) {
        return mTerrains[key]->getHeightAtPosition(position);
    }

    return 0.0f;
}

const std::map<std::string, Terrain*>& TerrainManager::getTerrains() const
{
    return mTerrains;
}

Terrain* TerrainManager::getTerrain(int x, int y)
{
    std::string key = std::to_string(x) + "." + std::to_string(y);
    if (mTerrains.find(key) != mTerrains.end()) {
        return mTerrains[key];
    }
    return nullptr;
}

void TerrainManager::raycast(const glm::vec3& position, const glm::vec3& direction, RaycastInfo& info)
{
    float range = 100.0f;

    float h = getHeightAtPosition(position);

    if (position.y < h) return;

    for (float t = 0; t < range; t += 0.1f) {
        glm::vec3 curPos = position + direction * t;

        h = getHeightAtPosition(curPos);
        if (curPos.y < h) {
            info.hit = true;
            info.position = curPos;
            break;
        }
    }
    
}

Terrain::Terrain()
{
    mMaterial = nullptr;
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    size = 100.0f;
}

void Terrain::init()
{
    mPlaneGeometry.scale = glm::vec3(size, 1.0f, size);
    mPlaneGeometry.init();
    mPlaneGeometry.computeTangents();
    mPlaneGeometry.initBuffers();

    computeModelMatrix();
}

void Terrain::loadFromHeightMap(const char* path, const HeightMapConfig& config)
{
    // Load the image
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    if (!ilLoadImage((wchar_t*)path)) {
        std::cout << "Error loading image: " << path << std::endl;
        return;
    }

    // Convert image to RGB format if it's not
    ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

    // Get image dimensions
    ILint imageW = ilGetInteger(IL_IMAGE_WIDTH);
    ILint imageH = ilGetInteger(IL_IMAGE_HEIGHT);

    // Access pixel data
    ILubyte* data = ilGetData();

    // Iterate over each pixel
    for (ILint y = 0; y < imageW; ++y) {
        for (ILint x = 0; x < imageH; ++x) {
            int index = (y * imageW + x) * 3; // RGB format
            ILubyte r = data[index];
            ILubyte g = data[index + 1];
            ILubyte b = data[index + 2];

            unsigned int coordX = x;
            unsigned int coordY = y;
            if (coordX < 0 || coordX > mPlaneGeometry.widthSegments || coordY < 0 || coordY > mPlaneGeometry.heightSegments) {
                continue;
            }

            float height = r / 255.0f * config.maxHeight;

            mPlaneGeometry.vertices[coordY * (mPlaneGeometry.widthSegments + 1) + coordX].position.y = height;
        }
    }

    mPlaneGeometry.computeTangents();

    mPlaneGeometry.recalculateNormals();
    mPlaneGeometry.initBuffers();

    // Cleanup
    ilDeleteImages(1, &imageID);
}

void Terrain::computeModelMatrix()
{
    mMatrix = glm::mat4(1.0f);
    mMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x + size * 0.5f, y, z + size * 0.5f)) * mMatrix;
}

void Terrain::draw(DrawCallbacks* drawCallbacks) const
{
    glBindVertexArray(mPlaneGeometry.getVAO());

    if (drawCallbacks) {
        drawCallbacks->supplyMaterial(*mMaterial);

        if (mMaterial->diffuseTexture) {
            mMaterial->diffuseTexture->bind(DIFFUSE);
            mMaterial->diffuseTextureRed->bind(DIFFUSE_RED);
            mMaterial->diffuseTextureGreen->bind(DIFFUSE_GREEN);
            mMaterial->diffuseTextureBlue->bind(DIFFUSE_BLUE);
            drawCallbacks->enableDiffuseTexture(true);
        }
        else {
            drawCallbacks->enableDiffuseTexture(false);
        }

        if (mMaterial->specularTexture) {
            mMaterial->specularTexture->bind(SPECULAR);
            drawCallbacks->enableSpecularTexture(true);
        }
        else {
            drawCallbacks->enableSpecularTexture(false);
        }

        if (mMaterial->normalMap) {
            mMaterial->normalMap->bind(NORMAL_MAP);
            mMaterial->normalMapRed->bind(NORMAL_MAP_RED);
            mMaterial->normalMapGreen->bind(NORMAL_MAP_GREEN);
            mMaterial->normalMapBlue->bind(NORMAL_MAP_BLUE);
            drawCallbacks->enableNormalMap(true);
        }
        else {
            drawCallbacks->enableNormalMap(false);
        }

        if (heightMap) {
            heightMap->bind(HEIGHT_MAP);
            drawCallbacks->enableHeightMap(true);
        }
        else {
            drawCallbacks->enableHeightMap(false);
        }
    }

    mPlaneGeometry.draw(0);

    glBindVertexArray(0);
}

void Terrain::setMaterial(TerrainMaterial* mat)
{
    mMaterial = mat;
}

float Terrain::getHeightAtPosition(const glm::vec3& position) const
{
    glm::vec3 localPosition = { position.x - x, position.y, position.z - z };

    float tileW = size / mPlaneGeometry.widthSegments;
    float tileH = size / mPlaneGeometry.heightSegments;

    int tileX = (int)floor(localPosition.x / tileW);
    int tileZ = (int)floor(localPosition.z / tileH);

    if (tileX < 0 || tileX >= mPlaneGeometry.widthSegments || tileZ < 0 || tileZ >= mPlaneGeometry.heightSegments) {
        return 0.0f;
    }

    float coordX = (localPosition.x - tileX * tileW) / tileW;
    float coordZ = (localPosition.z - tileZ * tileH) / tileH;

    float x0 = localPosition.x - size * 0.5f;
    float z0 = localPosition.z - size * 0.5f;

    if (coordX > coordZ) {
        const glm::vec3& p1 = mPlaneGeometry.vertices[tileZ * (mPlaneGeometry.widthSegments + 1) + tileX].position;
        const glm::vec3& p2 = mPlaneGeometry.vertices[tileZ * (mPlaneGeometry.widthSegments + 1) + tileX + 1].position;
        const glm::vec3& p3 = mPlaneGeometry.vertices[(tileZ + 1) * (mPlaneGeometry.widthSegments + 1) + tileX + 1].position;

        return y + calcBarrycentricCoordHeight(p1, p2, p3, x0, z0);
    }
    else {
        const glm::vec3& p1 = mPlaneGeometry.vertices[(tileZ + 1) * (mPlaneGeometry.widthSegments + 1) + tileX + 1].position;
        const glm::vec3& p2 = mPlaneGeometry.vertices[(tileZ + 1) * (mPlaneGeometry.widthSegments + 1) + tileX].position;
        const glm::vec3& p3 = mPlaneGeometry.vertices[tileZ * (mPlaneGeometry.widthSegments + 1) + tileX].position;

        return y + calcBarrycentricCoordHeight(p1, p2, p3, x0, z0);
    }

    return 0.0f;
}

void Terrain::setHeightMap(Texture* heightMap, const HeightMapConfig& config)
{
    loadFromHeightMap(heightMap->getPath().c_str(), config);
    this->heightMap = heightMap;
}
