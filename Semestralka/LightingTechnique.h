#pragma once

#include "ShaderTechnique.h"
#include "Material.h"

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

	GLuint mCameraPosLoc;

	struct {
		GLuint ambientColor;
		GLuint diffuseColor;
		GLuint specularColor;
	} mMaterialLoc;

	struct {
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint direction;
	} mDirLightLoc;
public:
	void init() override;
	LightingTechnique(Shader* shader);
	void supplyMaterial(const Material& material);
	void supplyDirLight(const DirectionalLight& dirLight);
	void supplyPointLights(const std::vector<PointLight>& pointLights);
	void supplySpotLights(const std::vector<SpotLight>& spotLights);
	void supplyPVMMatrix(const glm::mat4& PVMMatrix);
	void supplyModelMatrix(const glm::mat4& modelMatrix);
	void bindTextureUnits();
	void enableSpecularTexture(bool value);
	void supplyCameraPosition(const glm::vec3& pos);
	void prepare(const Game& game) override;
	void processGameObject(const GameObject& go) override;
};