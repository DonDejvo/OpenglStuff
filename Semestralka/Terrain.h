#pragma once
#include "Geometry.h"
#include <iostream>
#include "DrawCallbacks.h"

class Terrain {
private:
	PlaneGeometry mPlaneGeometry;
    glm::mat4 mMatrix;
    TerrainMaterial* mMaterial;
public:
    float x, z;
    float size;

    Terrain();

    struct HeightMapConfig {
        float maxHeight;
    };

    void init();

    void loadFromHeightMap(const char* path, HeightMapConfig config);

    void computeModelMatrix();

    inline const glm::mat4& getMatrix() const {
        return mMatrix;
    }

    void setSegments(int width, int height) {
        mPlaneGeometry.widthSegments = width;
        mPlaneGeometry.heightSegments = height;
    }

    void draw(DrawCallbacks* drawCallbacks) const;

    void setMaterial(TerrainMaterial* mat);

    float getHeightAtPosition(const glm::vec3& position) const;
};