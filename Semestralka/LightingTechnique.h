#pragma once

#include "ShaderTechnique.h"
#include "Material.h"
#include "Lights.h"

constexpr unsigned int NUM_TEX = 2;
constexpr unsigned int MAX_POINT_LIGHTS = 2;
constexpr unsigned int MAX_SPOT_LIGHTS = 2;

class LightingTechnique : public ShaderTechnique {
private:
	GLuint mTextureLoc[NUM_TEX];

	GLuint mNumPointLightsLoc;
	GLuint mNumSpotLightsLoc;

	struct AttenuationLoc {
		GLuint Const;
		GLuint Linear;
		GLuint Exp;
	};

	struct {
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint position;
		AttenuationLoc attenuation;
	} mPointLightsLoc[MAX_POINT_LIGHTS];

	struct {
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint position;
		GLuint direction;
		GLuint cutOff;
		AttenuationLoc attenuation;
	} mSpotLightsLoc[MAX_SPOT_LIGHTS];

	GLuint mSpecularEnabledLoc;
	GLuint mDiffuseEnabledLoc;

	GLuint mCameraPosLoc;

	struct {
		GLuint ambientColor;
		GLuint diffuseColor;
		GLuint specularColor;
		GLuint shininess;
	} mMaterialLoc;

	struct {
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint direction;
	} mDirLightLoc;
public:
	void init() override;
	void supplyMaterial(const Material& material) const;
	void supplyDirLight(const DirectionalLight& dirLight) const;
	void supplyPointLights(const std::vector<PointLight>& pointLights) const;
	void supplySpotLights(const std::vector<SpotLight>& spotLights) const;
	void supplyPVMMatrix(const glm::mat4& PVMMatrix) const;
	void supplyModelMatrix(const glm::mat4& modelMatrix) const;
	void bindTextureUnits() const;
	void enableSpecularTexture(bool value) const;
	void enableDiffuseTexture(bool value) const;
	void supplyCameraPosition(const glm::vec3& pos) const;
};