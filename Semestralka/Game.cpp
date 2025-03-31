#include "Game.h"

#include <iostream>
#include "Window.h"

#include "LightingTechnique.h"

Game::Game(const GameConfig& config): mConfig(config)
{
}

void Game::init()
{
    Shader* shader = new Shader();
    shader->loadFromFiles("shaders/simple.vert", "shaders/simple.frag");
    mShaders["simple"] = shader;

    Shader* lightingShader = new Shader();
    lightingShader->loadFromFiles("shaders/lighting.vert", "shaders/lighting.frag");
    mShaders["lighting"] = lightingShader;

    mTechnique = new LightingTechnique(lightingShader);

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

    mDirLight.color = glm::vec3(0.0f, 0.0f, 1.0f);
    mDirLight.ambientIntensity = 0.1f;
    mDirLight.diffuseIntensity = 0.9f;
    mDirLight.worldDirection = glm::vec3(0.0f, 0.0f, -1.0f);

    PointLight pointLight1;
    pointLight1.color = glm::vec3(1.0f, 0.0f, 0.0f);
    pointLight1.ambientIntensity = 0.1f;
    pointLight1.diffuseIntensity = 0.9f;
    pointLight1.position = glm::vec3(-15.0f, 15.0f, 10.0f);
    mPointLights.push_back(pointLight1);

    PointLight pointLight2;
    pointLight1.color = glm::vec3(0.0f, 1.0f, 0.0f);
    pointLight1.ambientIntensity = 0.1f;
    pointLight1.diffuseIntensity = 0.9f;
    pointLight1.position = glm::vec3(15.0f, 15.0f, 10.0f);
    mPointLights.push_back(pointLight1);

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
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

    mGameObjects[0]->yaw += 5.0f * dt;

    for (GameObject* go : mGameObjects) {
        go->update(dt);
    }
}

void Game::draw() const
{
    Window* win = Window::get();
    glViewport(0, 0, win->getWinWdth(), win->getWinHeight());

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    mTechnique->use();

    mTechnique->prepare(*this);

    for (GameObject* go : mGameObjects) {
        mTechnique->processGameObject(*go);
    }
}

void Game::destroy()
{
    
}

void Game::onKey(unsigned char keyPressed)
{
}
