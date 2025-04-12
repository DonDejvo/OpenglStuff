#include "TerrainTechnique.h"

void TerrainTechnique::init()
{
	LightingTechnique::init();

	mTextureLoc.resize(7);
	mTextureLoc[3] = glGetUniformLocation(mShader->getProgramID(), "u_TextureDiffuseRed");
	mTextureLoc[4] = glGetUniformLocation(mShader->getProgramID(), "u_TextureDiffuseGreen");
	mTextureLoc[5] = glGetUniformLocation(mShader->getProgramID(), "u_TextureDiffuseBlue");
	mTextureLoc[6] = glGetUniformLocation(mShader->getProgramID(), "u_BlendMap");
}
