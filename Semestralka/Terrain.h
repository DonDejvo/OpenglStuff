#pragma once
#include "Geometry.h"
#include <iostream>
#include "DrawCallbacks.h"
#include "Drawable.h"
#include <map>

class Terrain : public Drawable {
private:
	PlaneGeometry mPlaneGeometry;
    glm::mat4 mMatrix;
    TerrainMaterial* mMaterial;
    Texture* heightMap = nullptr;
public:
    float x, y, z;
    float size;

    Terrain();

    struct HeightMapConfig {
        float maxHeight;
    };

    void init();

    void loadFromHeightMap(const char* path, const HeightMapConfig& config);

    void computeModelMatrix();

    inline const glm::mat4& getMatrix() const override {
        return mMatrix;
    }

    void setSegments(int width, int height) {
        mPlaneGeometry.widthSegments = width;
        mPlaneGeometry.heightSegments = height;
    }

    void draw(DrawCallbacks* drawCallbacks) const override;

    void setMaterial(TerrainMaterial* mat);

    float getHeightAtPosition(const glm::vec3& position) const;

    void setHeightMap(Texture* heightMap, const HeightMapConfig& config);

    inline virtual const glm::vec3& getColor() const override {
        return glm::vec3(1.0f);
    }
};

class TerrainManager {
public:
    struct RaycastInfo {
        bool hit = false;
        glm::vec3 position;
    };

    int terrainSize;
    glm::ivec2 dimensions;
    TerrainMaterial* material;

    TerrainManager(const glm::vec2& dim, int terrainSize, TerrainMaterial* material);

    float getHeightAtPosition(const glm::vec3& position);
    const std::map<std::string, Terrain*>& getTerrains() const;
    Terrain* getTerrain(int x, int y);

    void raycast(const glm::vec3& position, const glm::vec3& direction, RaycastInfo& info);

private:

    std::map<std::string, Terrain*> mTerrains;
};