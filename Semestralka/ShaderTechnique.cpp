#include "ShaderTechnique.h"

void ShaderTechnique::init()
{
}

void ShaderTechnique::draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const
{
}

void ShaderTechnique::bindTextureUnits() const
{
	for (auto it = mTextureLoc.begin(); it != mTextureLoc.end(); ++it) {
		const auto& entry = *it;
		glActiveTexture(GL_TEXTURE0 + entry.first);
		glUniform1i(entry.second, entry.first);
	}
}

void ShaderTechnique::setShader(Shader* shader)
{
	mShader = shader;
}

void ShaderTechnique::use() const
{
	mShader->use();
}

