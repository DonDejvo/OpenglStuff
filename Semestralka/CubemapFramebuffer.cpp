#include "CubemapFramebuffer.h"

void CubemapFramebuffer::createCubemapAttachment(GLint internalFormat, GLenum minFilter, GLenum magFilter)
{
    glGenTextures(1, &mCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, Texture::getFormat(internalFormat), GL_FLOAT, NULL);
    }

    cubemapAttachmentIdx = attachmentCount;

    ++attachmentCount;
}

void CubemapFramebuffer::bindCubemap(GLuint cubemapUnit, GLenum unit) const
{
    glBindFramebuffer(unit, mFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + cubemapAttachmentIdx, cubemapUnit, mCubeMap, 0);
}

void CubemapFramebuffer::bindCubemapTexture(GLenum texUnit) const
{
    glActiveTexture(texUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMap);
}
