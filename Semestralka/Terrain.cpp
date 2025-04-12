#include "Terrain.h"
#include "mathUtils.h"

Terrain::Terrain()
{
    mMaterial = nullptr;
    x = 0.0f;
    z = 0.0f;
    size = 100.0f;
}

void Terrain::init()
{
    mPlaneGeometry.scale = glm::vec3(size, 1.0f, size);
    mPlaneGeometry.init();
    mPlaneGeometry.initBuffers();
}

void Terrain::loadFromHeightMap(const char* path, HeightMapConfig config)
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

            float height = (1.0f - r / 255.0f) * config.maxHeight;
            mPlaneGeometry.vertices[coordY * (mPlaneGeometry.widthSegments + 1) + coordX].position.y = height;
        }
    }

    mPlaneGeometry.recalculateNormals();
    mPlaneGeometry.initBuffers();

    // Cleanup
    ilDeleteImages(1, &imageID);
}

void Terrain::computeModelMatrix()
{
    mMatrix = glm::mat4(1.0f);
    mMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x + size * 0.5f, 0.0f, z + size * 0.5f)) * mMatrix;
}

void Terrain::draw(DrawCallbacks* drawCallbacks) const
{
    glBindVertexArray(mPlaneGeometry.getVAO());

    if (drawCallbacks) {
        drawCallbacks->supplyMaterial(*mMaterial);

        if (mMaterial->diffuseTexture) {
            mMaterial->diffuseTexture->bind(GL_TEXTURE0);
            mMaterial->diffuseTextureRed->bind(GL_TEXTURE3);
            mMaterial->diffuseTextureGreen->bind(GL_TEXTURE4);
            mMaterial->diffuseTextureBlue->bind(GL_TEXTURE5);
            mMaterial->blendMap->bind(GL_TEXTURE6);
            drawCallbacks->enableDiffuseTexture(true);
        }
        else {
            drawCallbacks->enableDiffuseTexture(false);
        }

        if (mMaterial->specularTexture) {
            mMaterial->specularTexture->bind(GL_TEXTURE1);
            drawCallbacks->enableSpecularTexture(true);
        }
        else {
            drawCallbacks->enableSpecularTexture(false);
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
    float tileW = size / mPlaneGeometry.widthSegments;
    float tileH = size / mPlaneGeometry.heightSegments;

    int tileX = (int)floor(position.x / tileW);
    int tileZ = (int)floor(position.z / tileH);

    if (tileX < 0 || tileX >= mPlaneGeometry.widthSegments || tileZ < 0 || tileZ >= mPlaneGeometry.heightSegments) {
        return 0.0f;
    }

    float coordX = (position.x - tileX * tileW) / tileW;
    float coordZ = (position.z - tileZ * tileH) / tileH;

    float x0 = position.x - x - size * 0.5f;
    float z0 = position.z - z - size * 0.5f;

    if (coordX > coordZ) {
        const glm::vec3& p1 = mPlaneGeometry.vertices[tileZ * (mPlaneGeometry.widthSegments + 1) + tileX].position;
        const glm::vec3& p2 = mPlaneGeometry.vertices[tileZ * (mPlaneGeometry.widthSegments + 1) + tileX + 1].position;
        const glm::vec3& p3 = mPlaneGeometry.vertices[(tileZ + 1) * (mPlaneGeometry.widthSegments + 1) + tileX + 1].position;

        return calcBarrycentricCoordHeight(p1, p2, p3, x0, z0);
    }
    else {
        const glm::vec3& p1 = mPlaneGeometry.vertices[(tileZ + 1) * (mPlaneGeometry.widthSegments + 1) + tileX + 1].position;
        const glm::vec3& p2 = mPlaneGeometry.vertices[(tileZ + 1) * (mPlaneGeometry.widthSegments + 1) + tileX].position;
        const glm::vec3& p3 = mPlaneGeometry.vertices[tileZ * (mPlaneGeometry.widthSegments + 1) + tileX].position;

        return calcBarrycentricCoordHeight(p1, p2, p3, x0, z0);
    }

    return 0.0f;
}
