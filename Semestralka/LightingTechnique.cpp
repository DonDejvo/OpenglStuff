#include "LightingTechnique.h"

void LightingTechnique::init()
{
	ShaderTechnique::init();

	mMaterialLoc.ambientColor = glGetUniformLocation(mShader->getProgramID(), "u_Material.AmbientColor");
	mMaterialLoc.diffuseColor = glGetUniformLocation(mShader->getProgramID(), "u_Material.DiffuseColor");
	mMaterialLoc.specularColor = glGetUniformLocation(mShader->getProgramID(), "u_Material.SpecularColor");

	mDirLightLoc.color = glGetUniformLocation(mShader->getProgramID(), "u_DirectionalLight.Base.Color");
	mDirLightLoc.ambientIntensity = glGetUniformLocation(mShader->getProgramID(), "u_DirectionalLight.Base.AmbientIntensity");
	mDirLightLoc.diffuseIntensity = glGetUniformLocation(mShader->getProgramID(), "u_DirectionalLight.Base.DiffuseIntensity");
	mDirLightLoc.direction = glGetUniformLocation(mShader->getProgramID(), "u_DirectionalLight.Direction");

	mTextureLoc[0] = glGetUniformLocation(mShader->getProgramID(), "u_TextureDiffuse");
	mTextureLoc[1] = glGetUniformLocation(mShader->getProgramID(), "u_TextureSpecular");

	mSpecularEnabledLoc = glGetUniformLocation(mShader->getProgramID(), "u_SpecularEnabled");
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
}

LightingTechnique::LightingTechnique(Shader* shader) : ShaderTechnique(shader)
{
	init();
}

void LightingTechnique::supplyMaterial(const Material& material)
{
	glUniform3f(mMaterialLoc.ambientColor, material.ambientColor.r, material.ambientColor.g, material.ambientColor.b);
	glUniform3f(mMaterialLoc.diffuseColor, material.diffuseColor.r, material.diffuseColor.g, material.diffuseColor.b);
	glUniform3f(mMaterialLoc.specularColor, material.specularColor.r, material.specularColor.g, material.specularColor.b);
}

void LightingTechnique::supplyDirLight(const DirectionalLight& dirLight)
{
	glUniform3f(mDirLightLoc.color, dirLight.color.r, dirLight.color.g, dirLight.color.b);
	glUniform1f(mDirLightLoc.ambientIntensity, dirLight.ambientIntensity);
	glUniform1f(mDirLightLoc.diffuseIntensity, dirLight.diffuseIntensity);
	glUniform3fv(mDirLightLoc.direction, 1, &glm::normalize(dirLight.worldDirection)[0]);
}

void LightingTechnique::supplyPointLights(const std::vector<PointLight>& pointLights)
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

void LightingTechnique::supplyPVMMatrix(const glm::mat4& PVMMatrix)
{
	glUniformMatrix4fv(PVMLocation, 1, GL_FALSE, &PVMMatrix[0][0]);
}

void LightingTechnique::supplyModelMatrix(const glm::mat4& modelMatrix)
{
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &modelMatrix[0][0]);
}

void LightingTechnique::bindTextureUnits()
{
	for (unsigned int i = 0; i < NUM_TEX; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glUniform1i(mTextureLoc[i], i);
	}
}

void LightingTechnique::enableSpecularTexture(bool value)
{
	glUniform1i(mSpecularEnabledLoc, value ? 1 : 0);
}

void LightingTechnique::supplyCameraPosition(const glm::vec3& pos)
{
	glUniform3fv(mCameraPosLoc, 1, &pos[0]);
}

void LightingTechnique::prepare(const Game& game)
{
	bindTextureUnits();
	supplyDirLight(game.getDirectionalLight());
	supplyPointLights(game.getPointLights());
	supplyCameraPosition(game.getCamera("main").position);
}

void LightingTechnique::processGameObject(const GameObject& go)
{

	glm::mat4 PVMMatrix = go.getCamera().getPVMatrix() * go.getMatrix();
	supplyModelMatrix(go.getMatrix());
	supplyPVMMatrix(PVMMatrix);

	go.getMesh().draw(this);
}
