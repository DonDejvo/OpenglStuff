#include "Game.h"

#include <iostream>
#include "Window.h"

Game::Game(const GameConfig& config): mConfig(config)
{
}

void Game::init()
{
    Shader* shader = new Shader();
    shader->loadFromFiles("shaders/simple.vert", "shaders/simple.frag");
    mShaders["simple"] = shader;

    technique = new ShaderTechnique(shader);

    Camera* camera = new PerspectiveCamera(Viewport(mConfig.winWidth, mConfig.winHeight), 3.14f / 3.0f, 0.1f, 1000.0f);
    mCameras["main"] = camera;
    camera->position.z = 10.0f;

    Camera* UICamera = new OrthoCamera(Viewport(mConfig.winWidth, mConfig.winHeight));
    mCameras["ui"] = UICamera;

    Mesh* mesh = new Mesh();
    mesh->loadFromFile("data/backpack/backpack.obj");
    GameObject* go = new GameObject(mesh, mCameras["main"]);
    go->scale = glm::vec3(1.0f);
    mGameObjects.push_back(go);

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void Game::update(float dt)
{
    Window* win = Window::get();

    for (auto it = mCameras.begin(); it != mCameras.end(); ++it) {
        Camera* camera = (*it).second;
        
        camera->onResize(win->getWinWdth(), win->getWinHeight());
        camera->update(dt);
    }

    mGameObjects[0]->yaw += 0.5f * dt;

    for (GameObject* go : mGameObjects) {
        go->update(dt);
    }
}

void Game::draw() const
{
    Window* win = Window::get();
    glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    technique->use();

    for (GameObject* go : mGameObjects) {
        go->draw(*technique);
    }
}

void Game::destroy()
{
    
}

void Game::onKey(unsigned char keyPressed)
{
}
