#include "Editor.h"
#include "Window.h"
#include "Dragon.h"
#include "Lamp.h"
#include "Horse.h"
#include "MageTower.h"
#include <fstream>
#include <sstream>

void Editor::initSprites(SimpleTechnique& technique)
{
	mMeshFramebuffer.bind();
	glViewport(0, 0, mMeshFramebuffer.width, mMeshFramebuffer.height);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_CULL_FACE);

	glm::mat4 projMat = glm::orthoRH(0.0f, (float)mMeshFramebuffer.width, (float)mMeshFramebuffer.height, 0.0f, -100.0f, 100.0f);

	technique.use();

	float size = mMeshFramebuffer.width / spritesheetCols;

	for (unsigned int i = 0; i < elements.size(); ++i) {
		unsigned int col = i % spritesheetCols;
		unsigned int row = i / spritesheetCols;

		Mesh mesh;
		mesh.loadFromFile(elements[i].path);
		mesh.position.x = (col + 0.5f) * size;
		mesh.position.y = (row + elements[i].offsetY) * size;
		mesh.scale *= elements[i].scale;
		mesh.computeModelMatrix();
		glm::mat4 matrix = projMat * mesh.getMatrix();
		technique.supplyPVMMatrix(matrix);
		mesh.draw(&technique);
	}

	mMeshFramebuffer.unbind();

	glEnable(GL_CULL_FACE);
}

Framebuffer& Editor::getFramebuffer()
{
	return mMeshFramebuffer;
}

void Editor::update(float dt)
{
	if (!enabled) return;

	counter += dt;
	if (counter > dur) {
		counter = 0.0f;
	}
	if (mSelectedObject == nullptr) {
		mTerrainManager->raycast(mCamera->position, mCamera->direction, info);
		if (info.hit) {
			mLineRenderer->addCircle(info.position, 1, { 0.0f, 0.0f, 1.0f, 1.0f }, 0, AI_MATH_HALF_PI, 16);
		}
	}
	else {
		mSelectedObject->getMesh()->position.y = mTerrainManager->getHeightAtPosition(mSelectedObject->getMesh()->position);
		if (mSelectedObject->type == "horse") {
			((Horse*)mSelectedObject)->initialPosition = mSelectedObject->getMesh()->position;
		}
		mSelectedObject->getMesh()->computeModelMatrix();
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f) * (2.0f - 2.0f * abs(0.5f - counter / dur));
		mSelectedObject->getMesh()->color = color;
		mLineRenderer->addLine(mSelectedObject->getMesh()->position, mSelectedObject->getMesh()->position - glm::vec3(sin(mSelectedObject->getMesh()->yaw), 0.0f, cos(mSelectedObject->getMesh()->yaw)), {0.0f, 0.0f, 1.0f, 1.0f});
	}
	mCameraMovement->update(dt);
	mPlayerMovement->update(dt);
	mPlayerCamera->update(dt);
}

void Editor::selectGameObject(GameObject* go)
{
	mSelectedObject = go;
	mPlayerCamera->setPlayer(go);
	mPlayerMovement->setPlayer(go);

	mCameraMovement->enabled = false;
	mPlayerCamera->enabled = true;
	mPlayerMovement->enabled = true;
}

void Editor::unselectGameObject()
{
	mSelectedObject = nullptr;

	mCameraMovement->enabled = true;
	mPlayerCamera->enabled = false;
	mPlayerMovement->enabled = false;
}

void Editor::saveLevel(App& game)
{
	std::ofstream ofs;
	ofs.open(levelPath, std::ios::trunc);
	if (!ofs.is_open()) {
		std::cout << "Could not open level file " << levelPath << "\n";
		return;
	}
	for (GameObject* go : game.gameObjects) {
		const glm::vec3& pos = go->getMesh()->position;
		ofs << go->type << " " << pos.x << " " << pos.z << " " << go->getMesh()->yaw << "\n";
	}
	ofs.close();
	std::cout << "Level saved successfully.\n";
}

void Editor::loadLevel(App& game)
{
	std::ifstream ifs;
	ifs.open(levelPath);
	if (!ifs.is_open()) {
		std::cout << "Could not open level file " << levelPath << "\n";
		return;
	}

	unselectGameObject();

	auto copy = game.gameObjects;
	for (GameObject* go : copy) {
		if (go->type == "player") continue;
		game.removeGameObject(go);
	}

	std::string line;
	while (std::getline(ifs, line)) {
		std::istringstream iss(line);

		std::string type;
		iss >> type;

		glm::vec3 pos;
		float yaw;
		iss >> pos.x;
		iss >> pos.z;
		iss >> yaw;

		if (type == "player") {
			GameObject* player = game.getObjectByName("player");
			player->getMesh()->position = pos;
			player->getMesh()->yaw = yaw;
		}
		else {
			insertGameObject(type, pos, yaw, game);
		}
	}
	ifs.close();

	std::cout << "Level loaded successfully\n";
}

void Editor::enable(bool value)
{
	if (value) {
		mCameraMovement->enabled = true;
	}
	else {
		unselectGameObject();
		mCameraMovement->enabled = false;
	}
	enabled = value;
}

void Editor::insertGameObject(const std::string& type, const glm::vec3& pos, float yaw, App& game)
{
	GameObject* go;
	if (type == "dragon") {
		go = new Dragon();
	}
	else if (type == "lamp") {
		go = new Lamp();
	}
	else if (type == "horse") {
		go = new Horse();
	}
	else if (type == "magetower") {
		go = new MageTower();
	}
	else {
		go = new GameObject();
		if (type == "fern") {
			go->collidable = false;
		}
		else if (type == "rock") {
			go->radius = 2.0f;
		}

		go->getMesh()->loadFromFile(getPathByType(type));
	}
	go->type = type;
	go->getMesh()->position = pos;
	go->getMesh()->yaw = yaw;
	go->getMesh()->position.y = mTerrainManager->getHeightAtPosition(go->getMesh()->position);
	game.addGameObject(go);
}

const std::string& Editor::getPathByType(const std::string& type) const
{
	for (const auto& elem : elements) {
		if (elem.type == type) {
			return elem.path;
		}
	}
	return "";
}

Editor::Editor(UIElements* UIElements, LineRenderer* lineRenderer, TerrainManager* terrainManager, Camera* cam)
	: mUIElements(UIElements), mLineRenderer(lineRenderer), mTerrainManager(terrainManager), mCamera(cam)
{
	mMeshFramebuffer.width = 256;
	mMeshFramebuffer.height = 256;
	mMeshFramebuffer.init();
	mMeshFramebuffer.createAttachment(GL_RGB8);

	mCameraMovement = new CameraMovement();
	mCameraMovement->setCamera(cam);
	mCameraMovement->enabled = false;

	mPlayerCamera = new PlayerCamera(nullptr, cam);
	mPlayerCamera->enabled = false;
	mPlayerMovement = new PlayerMovement(nullptr);
	mPlayerMovement->enabled = false;
	mPlayerMovement->moveSpeed = 40.0f;

	elements.push_back({
		"laubbaum",
		"data/trees/laubbaum.obj",
		8
	});
	elements.push_back({
		"tree",
		"data/trees/tree.obj",
		8
		});
	elements.push_back({
		"tree_snow",
		"data/trees/tree_snow.obj",
		8
		});
	elements.push_back({
		"fern",
		"data/fern/fern1.obj",
		8
	});
	elements.push_back({
		"lamp",
		"data/lamp/StreetLamp.obj",
		3.0f,
		0.25f
		});
	elements.push_back({
		"rock",
		"data/rock/roca.obj",
		8
		});
	elements.push_back({
		"horse",
		"data/horse/CABALLO_low_poly.obj",
		8,
		0.5f
		});
	elements.push_back({
		"dragon",
		"data/dragon/jadedragon.obj",
		7,
		0.15f
		});
	elements.push_back({
			"magetower",
			"data/magetower/mage_tower.obj",
			7,
			0.05f
		});
}

void Editor::drawUI(App& game)
{
	Window* win = Window::get();

	if (!opened) {
		if (mUIElements->button("Editor", win->getWinWdth() - 200, 10)) {
			opened = true;
		}
	}
	else {
		mUIElements->begin(win->getWinWdth() - 620, 10, 600, 400);
		float y = 0.0f;

		if (mUIElements->button("X", 600 - 40, y, 40, 40)) {
			opened = false;
		}

		y += 40.0f;

		if (enabled == false) {
			mUIElements->getCanvas()->text("Editor is disabled!", 0, y);
		}
		else {
			if (mSelectedObject == nullptr) {
				float size = 1.0f / spritesheetCols;
				for (unsigned int i = 0; i < elements.size(); ++i) {
					unsigned int col = i % spritesheetCols;
					unsigned int row = i / spritesheetCols;
					if (mUIElements->imageButton(&mMeshFramebuffer.getAttachment(0), col * size, 1.0f - (row + 1) * size, size, size, (i % 5) * 68, y, 64, 64)) {
						if (info.hit) {
							insertGameObject(elements[i].type, info.position, 0, game);
						}
					}
					if (i % 5 == 4) {
						y += 68;
					}
				}
			}
			else {
				mUIElements->getCanvas()->text("Selected object", 0, y);

				y += 40.0f;

				if (mUIElements->button("Unselect", 0, y, 200, 40)) {
					unselectGameObject();
				}
				else if (mSelectedObject->type != "player" && mUIElements->button("Delete", 210, y, 200, 40)) {
					GameObject* go = mSelectedObject;
					unselectGameObject();
					game.removeGameObject(go);
				}
			}

			y += 80.0f;

			if (mUIElements->button("Save", 0, y, 200, 40)) {
				saveLevel(game);
			}
			if (mUIElements->button("Load", 220, y, 200, 40)) {
				loadLevel(game);
			}
		}

		mUIElements->end();
	}
}
