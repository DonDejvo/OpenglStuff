#pragma once

#include "ShaderTechnique.h"
#include "Framebuffer.h"

class PickingTechnique : public ShaderTechnique {
protected:
	GLuint mPVMMatrixLoc;
	GLuint mObjectIDLoc;
	Framebuffer mFramebuffer;
public:
	struct Pixel {
		unsigned int ObjectID = 0;
		unsigned int PrimitiveID = 0;
	};
	void init() override;
	void supplyPVMMatrix(const glm::mat4& matrix) const;
	void supplyObjectID(unsigned int id) const;
	virtual void draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const override;
	void enableWriting() const;
	void disableWriting() const;
	void readPixel(int mouseX, int mouseY, Pixel& pixel) const;
};