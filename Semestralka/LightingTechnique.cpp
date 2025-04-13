#include "LightingTechnique.h"

void LightingTechnique::init()
{
	FogTechnique::init();

	PVMLocation = glGetUniformLocation(mShader->getProgramID(), "u_PVM");
	modelLocation = glGetUniformLocation(mShader->getProgramID(), "u_Model");
	lightPVMLocation = glGetUniformLocation(mShader->getProgramID(), "u_LightPVM");

	mMaterialLoc.ambientColor = glGetUniformLocation(mShader->getProgramID(), "u_Material.AmbientColor");
	mMaterialLoc.diffuseColor = glGetUniformLocation(mShader->getProgramID(), "u_Material.DiffuseColor");
	mMaterialLoc.specularColor = glGetUniformLocation(mShader->getProgramID(), "u_Material.SpecularColor");
	mMaterialLoc.shininess = glGetUniformLocation(mShader->getProgramID(), "u_Material.Shininess");

	mDirLightLoc.color = glGetUniformLocation(mShader->getProgramID(), "u_DirectionalLight.Base.Color");
	mDirLightLoc.ambientIntensity = glGetUniformLocation(mShader->getProgramID(), "u_DirectionalLight.Base.AmbientIntensity");
	mDirLightLoc.diffuseIntensity = glGetUniformLocation(mShader->getProgramID(), "u_DirectionalLight.Base.DiffuseIntensity");
	mDirLightLoc.direction = glGetUniformLocation(mShader->getProgramID(), "u_DirectionalLight.Direction");

	mTextureLoc[DIFFUSE - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_TextureDiffuse");
	mTextureLoc[SPECULAR - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_TextureSpecular");
	mTextureLoc[SHADOW_MAP - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_ShadowMap");
	mTextureLoc[NORMAL_MAP - GL_TEXTURE0] = glGetUniformLocation(mShader->getProgramID(), "u_NormalMap");

	mSpecularEnabledLoc = glGetUniformLocation(mShader->getProgramID(), "u_SpecularEnabled");
	mDiffuseEnabledLoc = glGetUniformLocation(mShader->getProgramID(), "u_DiffuseTextureEnabled");
	mNormalMapEnabledLoc = glGetUniformLocation(mShader->getProgramID(), "u_NormalMapEnabled");

	mCameraPosLoc = glGetUniformLocation(mShader->getProgramID(), "u_CameraPosition");

	mNumPointLightsLoc = glGetUniformLocation(mShader->getProgramID(), "u_NumPointLights");
	mNumSpotLightsLoc = glGetUniformLocation(mShader->getProgramID(), "u_NumSpotLights");

	for (unsigned int i = 0; i < MAX_POINT_LIGHTS; ++i) {
		mPointLightsLoc[i].color = glGetUniformLocation(mShader->getProgramID(), ("u_PointLights[" + std::to_string(i) + "].Base.Color").c_str());
		mPointLightsLoc[i].ambientIntensity = glGetUniformLocation(mShader->getProgramID(), ("u_PointLights[" + std::to_string(i) + "].Base.AmbientIntensity").c_str());
		mPointLightsLoc[i].diffuseIntensity = glGetUniformLocation(mShader->getProgramID(), ("u_PointLights[" + std::to_string(i) + "].Base.DiffuseIntensity").c_str());
		mPointLightsLoc[i].position = glGetUniformLocation(mShader->getProgramID(), ("u_PointLights[" + std::to_string(i) + "].Position").c_str());
		mPointLightsLoc[i].attenuation.Const = glGetUniformLocation(mShader->getProgramID(), ("u_PointLights[" + std::to_string(i) + "].Attenuation.Const").c_str());
		mPointLightsLoc[i].attenuation.Linear = glGetUniformLocation(mShader->getProgramID(), ("u_PointLights[" + std::to_string(i) + "].Attenuation.Linear").c_str());
		mPointLightsLoc[i].attenuation.Exp = glGetUniformLocation(mShader->getProgramID(), ("u_PointLights[" + std::to_string(i) + "].Attenuation.Exp").c_str());
	}

	for (unsigned int i = 0; i < MAX_SPOT_LIGHTS; ++i) {
		mSpotLightsLoc[i].color = glGetUniformLocation(mShader->getProgramID(), ("u_SpotLights[" + std::to_string(i) + "].Base.Base.Color").c_str());
		mSpotLightsLoc[i].ambientIntensity = glGetUniformLocation(mShader->getProgramID(), ("u_SpotLights[" + std::to_string(i) + "].Base.Base.AmbientIntensity").c_str());
		mSpotLightsLoc[i].diffuseIntensity = glGetUniformLocation(mShader->getProgramID(), ("u_SpotLights[" + std::to_string(i) + "].Base.Base.DiffuseIntensity").c_str());
		mSpotLightsLoc[i].position = glGetUniformLocation(mShader->getProgramID(), ("u_SpotLights[" + std::to_string(i) + "].Base.Position").c_str());
		mSpotLightsLoc[i].attenuation.Const = glGetUniformLocation(mShader->getProgramID(), ("u_SpotLights[" + std::to_string(i) + "].Base.Attenuation.Const").c_str());
		mSpotLightsLoc[i].attenuation.Linear = glGetUniformLocation(mShader->getProgramID(), ("u_SpotLights[" + std::to_string(i) + "].Base.Attenuation.Linear").c_str());
		mSpotLightsLoc[i].attenuation.Exp = glGetUniformLocation(mShader->getProgramID(), ("u_SpotLights[" + std::to_string(i) + "].Base.Attenuation.Exp").c_str());
		mSpotLightsLoc[i].direction = glGetUniformLocation(mShader->getProgramID(), ("u_SpotLights[" + std::to_string(i) + "].Direction").c_str());
		mSpotLightsLoc[i].cutOff  = glGetUniformLocation(mShader->getProgramID(), ("u_SpotLights[" + std::to_string(i) + "].CutOff").c_str());
	}
}

void LightingTechnique::supplyMaterial(const Material& material) const
{
	glUniform3f(mMaterialLoc.ambientColor, material.ambientColor.r, material.ambientColor.g, material.ambientColor.b);
	glUniform3f(mMaterialLoc.diffuseColor, material.diffuseColor.r, material.diffuseColor.g, material.diffuseColor.b);
	glUniform3f(mMaterialLoc.specularColor, material.specularColor.r, material.specularColor.g, material.specularColor.b);
	glUniform1f(mMaterialLoc.shininess, material.shininess);
}

void LightingTechnique::supplyDirLight(const DirectionalLight& dirLight) const
{
	glUniform3f(mDirLightLoc.color, dirLight.color.r, dirLight.color.g, dirLight.color.b);
	glUniform1f(mDirLightLoc.ambientIntensity, dirLight.ambientIntensity);
	glUniform1f(mDirLightLoc.diffuseIntensity, dirLight.diffuseIntensity);
	glUniform3fv(mDirLightLoc.direction, 1, &glm::normalize(dirLight.worldDirection)[0]);
}

void LightingTechnique::supplyPointLights(const std::vector<PointLight>& pointLights) const
{
	glUniform1i(mNumPointLightsLoc, pointLights.size());
	for (unsigned int i = 0; i < pointLights.size(); ++i) {
		const PointLight& light = pointLights[i];

		glUniform3f(mPointLightsLoc[i].color, light.color.r, light.color.g, light.color.b);
		glUniform1f(mPointLightsLoc[i].ambientIntensity, light.ambientIntensity);
		glUniform1f(mPointLightsLoc[i].diffuseIntensity, light.diffuseIntensity);
		glUniform3fv(mPointLightsLoc[i].position, 1, &light.position[0]);
		glUniform1f(mPointLightsLoc[i].attenuation.Const, light.attenuation.Const);
		glUniform1f(mPointLightsLoc[i].attenuation.Linear, light.attenuation.Linear);
		glUniform1f(mPointLightsLoc[i].attenuation.Exp, light.attenuation.Exp);
	}
}

void LightingTechnique::supplySpotLights(const std::vector<SpotLight>& spotLights) const
{
	glUniform1i(mNumSpotLightsLoc, spotLights.size());
	for (unsigned int i = 0; i < spotLights.size(); ++i) {
		const SpotLight& light = spotLights[i];

		glUniform3f(mSpotLightsLoc[i].color, light.color.r, light.color.g, light.color.b);
		glUniform1f(mSpotLightsLoc[i].ambientIntensity, light.ambientIntensity);
		glUniform1f(mSpotLightsLoc[i].diffuseIntensity, light.diffuseIntensity);
		glUniform3fv(mSpotLightsLoc[i].position, 1, &light.position[0]);
		glUniform1f(mSpotLightsLoc[i].attenuation.Const, light.attenuation.Const);
		glUniform1f(mSpotLightsLoc[i].attenuation.Linear, light.attenuation.Linear);
		glUniform1f(mSpotLightsLoc[i].attenuation.Exp, light.attenuation.Exp);
		glUniform3fv(mSpotLightsLoc[i].direction, 1, &glm::normalize(light.direction)[0]);
		glUniform1f(mSpotLightsLoc[i].cutOff, light.cutOff);
	}
}

void LightingTechnique::supplyPVMMatrix(const glm::mat4& PVMMatrix) const
{
	glUniformMatrix4fv(PVMLocation, 1, GL_FALSE, &PVMMatrix[0][0]);
}

void LightingTechnique::supplyLightPVMMatrix(const glm::mat4& PVMMatrix) const
{
	glUniformMatrix4fv(lightPVMLocation, 1, GL_FALSE, &PVMMatrix[0][0]);
}

void LightingTechnique::supplyModelMatrix(const glm::mat4& modelMatrix) const
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
}

void LightingTechnique::bindTextureUnits() const
{
	for (auto it = mTextureLoc.begin(); it != mTextureLoc.end(); ++it) {
		const auto& entry = *it;
		glActiveTexture(GL_TEXTURE0 + entry.first);
		glUniform1i(entry.second, entry.first);
	}
}

void LightingTechnique::enableSpecularTexture(bool value) const
{
	glUniform1i(mSpecularEnabledLoc, value ? 1 : 0);
}

void LightingTechnique::enableDiffuseTexture(bool value) const
{
	glUniform1i(mDiffuseEnabledLoc, value ? 1 : 0);
}

void LightingTechnique::enableNormalMap(bool value) const
{
	glUniform1i(mNormalMapEnabledLoc, value ? 1 : 0);
}

void LightingTechnique::supplyCameraPosition(const glm::vec3& pos) const
{
	glUniform3fv(mCameraPosLoc, 1, &pos[0]);
}
