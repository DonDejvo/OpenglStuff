#include "Window.h"

#include "pgr.h"
#include "Input.h"
#include <iostream>

Window* Window::instance = nullptr;

void Window::onDisplay()
{
    instance->mGame->draw();
    glutSwapBuffers();
}

void Window::onClose()
{
    instance->mGame->destroy();
}

void Window::onTimer(int)
{
    instance->update();

    glutTimerFunc(WIN_FRAME_RATE, onTimer, 0);

    glutPostRedisplay();
}

void Window::onKeyboard(unsigned char key, int mouseX, int mouseY)
{
    Input::get()->onKeyDown(key);
}

void Window::onKeyboardUp(unsigned char key, int mouseX, int mouseY)
{
    Input::get()->onKeyUp(key);
}

void Window::onReshape(int newWidth, int newHeight)
{
    instance->mWinWidth = newWidth;
    instance->mWinHeight = newHeight;
}

void Window::onMouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        Input::get()->onMouseDown(button);
    }
    else if (state == GLUT_UP) {
        Input::get()->onMouseUp(button);
    }
}

void Window::onPassiveMouseMotion(int mouseX, int mouseY)
{
    Input::get()->onMouseMove(mouseX, mouseY);
}

void Window::onMouseMotion(int x, int y)
{
    Input::get()->onMouseMove(x, y);
}

void Window::onMouseWheel(int wheel, int direction, int x, int y)
{
    Input::get()->onScroll(direction);
}

void Window::start(int argc, char* argv[], Game* game)
{
    const GameConfig& config = game->getConfig();

    glutInit(&argc, argv);

    glutInitContextVersion(GL_VERSION_MAJOR, GL_VERSION_MINOR);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(config.winWidth, config.winHeight);

    glutCreateWindow(config.title);

    glutDisplayFunc(onDisplay);
    glutCloseFunc(onClose);
    glutTimerFunc(WIN_FRAME_RATE, onTimer, 0);
    glutReshapeFunc(onReshape);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMouseFunc(onMouse);
    glutPassiveMotionFunc(onPassiveMouseMotion);
    glutMotionFunc(onMouseMotion);
    glutMouseWheelFunc(onMouseWheel);

    if (!pgr::initialize(GL_VERSION_MAJOR, GL_VERSION_MINOR)) {
        pgr::dieWithError("pgr init failed, required OpenGL not supported.");
    }

    Window win(game, config.winWidth, config.winHeight);

    instance = &win;

    glutMainLoop();
}

Window* Window::get()
{
    return instance;
}

Window::Window(Game* game, int winWidth, int winHeight):
    mGame(game), mWinWidth(winWidth), mWinHeight(winHeight) {
	
    mPrevTime = 0.0f;

    mGame->init();
}

void Window::update()
{
    float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
    if (mPrevTime == 0.0f) {
        mPrevTime = currentTime;
    }
    float dt = currentTime - mPrevTime;
    mPrevTime = currentTime;

    mGame->update(dt);

    Input::get()->update();
}
