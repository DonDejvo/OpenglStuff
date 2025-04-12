#include "SkyBox.h"
#include <iostream>

void SkyBox::loadFromFiles(const std::vector<std::string>& paths)
{
	glGenTextures(1, &mTexID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexID);

    for (unsigned int i = 0; i < paths.size(); ++i) {
        ILuint imageID;

        ilGenImages(1, &imageID);
        ilBindImage(imageID);

        if (!ilLoadImage((wchar_t*)paths[i].c_str())) {
            ilDeleteImages(1, &imageID);
            std::cout << "ERROR: Image " << paths[i] << " could not be loaded.\n";
        }

        ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); // Convert to RGBA

        // Upload texture to GPU
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, ilGetData());

        ilDeleteImages(1, &imageID);
        //pgr::loadTexImage2D(paths[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void SkyBox::bind(GLenum texUnit) const
{
    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexID);
}
