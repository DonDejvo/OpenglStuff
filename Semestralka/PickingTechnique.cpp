#include "PickingTechnique.h"
#include <iostream>
#include "Window.h"

void PickingTechnique::init()
{
	ShaderTechnique::init();

	mPVMMatrixLoc = glGetUniformLocation(mShader->getProgramID(), "u_PVM");
	mObjectIDLoc = glGetUniformLocation(mShader->getProgramID(), "u_ObjectID");

	mFramebuffer.width = 1024;
	mFramebuffer.height = 1024;
	mFramebuffer.init();
	mFramebuffer.createAttachment(GL_RGB8UI, GL_NEAREST, GL_NEAREST);
	mFramebuffer.createDepthBuffer();
}

void PickingTechnique::supplyPVMMatrix(const glm::mat4& matrix) const
{
	glUniformMatrix4fv(mPVMMatrixLoc, 1, GL_FALSE, &matrix[0][0]);
}

void PickingTechnique::supplyObjectID(unsigned int id) const
{
	glUniform1ui(mObjectIDLoc, id);
}

void PickingTechnique::draw(const Drawable& drawable, const std::vector<Camera*>& cameras) const
{
	supplyObjectID(drawable.getID());

	glm::mat4 matrix = cameras[0]->getPVMatrix() * drawable.getMatrix();
	supplyPVMMatrix(matrix);

	drawable.draw((DrawCallbacks*)this);
}

void PickingTechnique::enableWriting() const
{
	mFramebuffer.bind(GL_DRAW_FRAMEBUFFER);
	glViewport(0, 0, mFramebuffer.width, mFramebuffer.height);
}

void PickingTechnique::disableWriting() const
{
	mFramebuffer.unbind(GL_DRAW_FRAMEBUFFER);
}

void PickingTechnique::readPixel(int mouseX, int mouseY, Pixel& pixel) const
{
	Window* win = Window::get();

	unsigned int x = (unsigned int)((float)mouseX / win->getWinWdth() * mFramebuffer.width);
	unsigned int y = (unsigned int)((1.0f - (float)mouseY / win->getWinHeight()) * mFramebuffer.height);

	mFramebuffer.bind(GL_READ_FRAMEBUFFER);
	glReadBuffer(GL_COLOR_ATTACHMENT0);

	unsigned int data[3];
	glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &data);

	pixel.ObjectID = data[0];
	pixel.PrimitiveID = data[1];

	glReadBuffer(GL_NONE);
	mFramebuffer.unbind(GL_READ_FRAMEBUFFER);
}
