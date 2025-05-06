#pragma once

#include "pgr.h"

#include "ShaderTechnique.h"
#include "Geometry.h"
#include <vector>
#include "Spritesheet.h"

constexpr unsigned int MAX_PARTICLES = 10000;

struct ParticleAnim {
	Spritesheet* spritesheet;
	std::vector<unsigned int> frames;
};

class Particle {
private:
	float elapsedTime;

	void updateTexCoordInfo();
public:
	ParticleAnim* anim;
	glm::vec3 position;
	float size;
	float angle;

	float lifeTime;
	glm::vec3 velocity;
	SpriteRegion spriteRegion;

	Particle(const glm::vec3& position, float size, float angle, float lifeTime, ParticleAnim* anim);

	void update(float dt);
	bool isAlive() const;
};

class ParticleTechnique : public ShaderTechnique {
private:
	struct DataVertex {
		glm::mat4 viewModel;
		float spriteRegion[4];
	};

	unsigned int offset = 0;
	DataVertex data[MAX_PARTICLES];

	QuadGeometry mGeom;
	GLuint mProjMatrixLoc, mViewModelMatrixLoc;
	GLuint mSpritesheetInfoLoc;
	GLuint mSpriteIndexLoc;
	GLuint mBlendFactorLoc;
	GLuint mDataVBO;

	void updateMatrix(const glm::vec3& position, float angle, float scale, const glm::mat4& ProjViewMatrix);
public:
	void init() override;
	void supplySpritesheetInfo(const Spritesheet& spritesheet) const;
	void draw(const std::vector<Particle>& particles, Camera* camera);
};