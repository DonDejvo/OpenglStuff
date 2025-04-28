#include "ParticleSystem.h"

void ParticleTechnique::updateMatrix(const glm::vec3& position, float angle, float scale, const glm::mat4& ViewMatrix)
{
	DataVertex& v = data[offset];

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	for (unsigned int i = 0; i < 3; ++i) {
		for (unsigned int j = 0; j < 3; ++j) {
			modelMatrix[i][j] = ViewMatrix[j][i];
		}
	}
	modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

	v.viewModel = ViewMatrix * modelMatrix;

	//glUniformMatrix4fv(mViewModelMatrixLoc, 1, GL_FALSE, &v.viewModel[0][0]);
}

void ParticleTechnique::init()
{
	ShaderTechnique::init();

	mGeom.init();
	mGeom.initBuffers();

	mProjMatrixLoc = glGetUniformLocation(mShader->getProgramID(), "u_Proj");
	mViewModelMatrixLoc = glGetUniformLocation(mShader->getProgramID(), "u_ViewModel");

	mSpritesheetInfoLoc = glGetUniformLocation(mShader->getProgramID(), "u_SpritesheetInfo");
	mSpriteIndexLoc = glGetUniformLocation(mShader->getProgramID(), "u_SpriteIndex");
	mBlendFactorLoc = glGetUniformLocation(mShader->getProgramID(), "u_BlendFactor");

	glBindVertexArray(mGeom.getVAO());

	glGenBuffers(1, &mDataVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mDataVBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(DataVertex), nullptr, GL_STREAM_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(DataVertex), (void*)offsetof(DataVertex, viewModel));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(DataVertex), (void*)(offsetof(DataVertex, viewModel) + 4 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(DataVertex), (void*)(offsetof(DataVertex, viewModel) + 8 * sizeof(float)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(DataVertex), (void*)(offsetof(DataVertex, viewModel) + 12 * sizeof(float)));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(DataVertex), (void*)offsetof(DataVertex, spriteRegion));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void ParticleTechnique::supplySpritesheetInfo(const Spritesheet& spritesheet) const
{
	spritesheet.tex->bind(DIFFUSE);
}

void ParticleTechnique::draw(const std::vector<Particle>& particles, Camera* camera)
{
	glm::mat4 viewMatrix = camera->getViewMatrix();
	glm::mat4 projMatrix = camera->getProjMatrix();

	offset = 0;

	for (const Particle& p : particles) {
		if (offset >= MAX_PARTICLES) {
			break;
		}
		DataVertex& v = data[offset];
		//glUniform2i(mSpriteIndexLoc, p.currentSpriteIdx[0], p.currentSpriteIdx[1]);
		//glUniform1f(mBlendFactorLoc, p.blend);
		v.spriteRegion[0] = p.spriteRegion.x;
		v.spriteRegion[1] = p.spriteRegion.y;
		v.spriteRegion[2] = p.spriteRegion.w;
		v.spriteRegion[3] = p.spriteRegion.h;
		updateMatrix(p.position, p.angle, p.size, viewMatrix);
		//mGeom.draw(0);
		++offset;
	}

	glBindBuffer(GL_ARRAY_BUFFER, mDataVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(DataVertex) * offset, data);

	glBindVertexArray(mGeom.getVAO());

	glUniformMatrix4fv(mProjMatrixLoc, 1, GL_FALSE, &projMatrix[0][0]);
	mGeom.drawInstanced(offset, 0);

	glBindVertexArray(0);

}

void Particle::updateTexCoordInfo()
{
	float lifeFactor = elapsedTime / lifeTime * spriteIndices.size();

	unsigned int idx = (unsigned int)floor(lifeFactor);
	idx = std::min(idx, spriteIndices.size() - 1);

	unsigned int currentSpriteIdx = spriteIndices[idx];
	spriteRegion = spritesheet->getRegion(currentSpriteIdx);
}

Particle::Particle(const glm::vec3& position, float size, float angle, float lifeTime)
	: position(position), size(size), angle(angle), lifeTime(lifeTime)
{
	elapsedTime = 0.0f;
	velocity = glm::vec3(0.0f);
}

void Particle::update(float dt)
{
	position += velocity * dt;

	elapsedTime += dt;

	updateTexCoordInfo();
}

bool Particle::isAlive() const
{
	return elapsedTime < lifeTime;
}
