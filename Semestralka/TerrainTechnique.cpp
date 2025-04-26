#include "TerrainTechnique.h"

void TerrainTechnique::init()
{
	LightingTechnique::init();

	mTextureLoc[DIFFUSE_RED - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_TextureDiffuseRed");
	mTextureLoc[DIFFUSE_GREEN - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_TextureDiffuseGreen");
	mTextureLoc[DIFFUSE_BLUE - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_TextureDiffuseBlue");
	mTextureLoc[BLEND_MAP - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_BlendMap");
	mTextureLoc[HEIGHT_MAP - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_HeightMap");
	mTextureLoc[NORMAL_MAP_RED - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_NormalMapRed");
	mTextureLoc[NORMAL_MAP_GREEN - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_NormalMapGreen");
	mTextureLoc[NORMAL_MAP_BLUE - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_NormalMapBlue");
}
