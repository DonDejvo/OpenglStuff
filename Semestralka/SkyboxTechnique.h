#pragma once

#include "ShaderTechnique.h"
#include "Geometry.h"

class SkyboxTechnique : public ShaderTechnique {
private:
	Geometry *mGeometry;
	GLuint mPVMatrixLoc;
public:
	void init() override;
	void supplyPVMatrix(const glm::mat4& matrix) const;
    void draw() const;
	inline void setGeomtry(Geometry* geom) {
		mGeometry = geom;
	}
};