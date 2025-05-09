#pragma once

#include <vector>
#include <map>
#include "FogTechnique.h"
#include "Material.h"
#include "Lights.h"

constexpr unsigned int MAX_POINT_LIGHTS = 16;
constexpr unsigned int MAX_SPOT_LIGHTS = 16;

class LightingTechnique : public FogTechnique {
protected:
	GLuint modelLocation;
	GLuint lightPVMLocation;

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
	GLuint mNormalMapEnabledLoc;

	GLuint mCameraPosLoc;

	struct {
		GLuint ambientColor;
		GLuint diffuseColor;
		GLuint specularColor;
		GLuint shininess;
		GLuint alpha;
		GLuint bidiretionalNormals;
	} mMaterialLoc;

	struct {
		GLuint color;
		GLuint ambientIntensity;
		GLuint diffuseIntensity;
		GLuint direction;
	} mDirLightLoc;

	GLuint mClipPlaneLoc;
public:
	void init() override;
	void supplyMaterial(const Material& material) const override;
	void supplyDirLight(const DirectionalLight& dirLight) const;
	void supplyPointLights(const std::vector<PointLight*>& pointLights) const;
	void supplySpotLights(const std::vector<SpotLight*>& spotLights) const;
	void supplyLightPVMMatrix(const glm::mat4& PVMMatrix) const;
	void supplyModelMatrix(const glm::mat4& modelMatrix) const;
	void enableSpecularTexture(bool value) const override;
	void enableDiffuseTexture(bool value) const override;
	void enableNormalMap(bool value) const override;
	void supplyCameraPosition(const glm::vec3& pos) const;
	void supplyClipPlane(const glm::vec4& plane) const;
	void draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const override;
	void supplyColor(const glm::vec3& color) const;
};