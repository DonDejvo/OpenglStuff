#pragma once
#include "GameObject.h"
#include "UIElements.h"
#include "SimpleTechnique.h"
#include "Framebuffer.h"
#include "Terrain.h"
#include "CameraMovement.h"
#include "PlayerMovement.h"
#include "PlayerCamera.h"
#include "LineRenderer.h"

class Editor {
private:
	UIElements* mUIElements;
	LineRenderer* mLineRenderer;
	GameObject* mSelectedObject = nullptr;
	TerrainManager* mTerrainManager;
	Camera* mCamera;
	Framebuffer mMeshFramebuffer;
	CameraMovement* mCameraMovement;
	unsigned int spritesheetCols = 4;
	PlayerMovement* mPlayerMovement;
	PlayerCamera* mPlayerCamera;
	TerrainManager::RaycastInfo info;
	float counter = 0.0f;
	float dur = 1.0f;

	struct EditorElement {
		std::string type;
		std::string path;
		float scale;
		float offsetY = 0.0f;
	};
	std::vector<EditorElement> elements;

	void insertGameObject(const std::string& type, const glm::vec3& pos, float yaw, App& game);
	const std::string& getPathByType(const std::string& type) const;
public:
	std::string levelPath = "level.txt";
	bool opened = false;
	bool enabled = false;

	Editor(UIElements* UIElements, LineRenderer* lineRenderer, TerrainManager* terrainManager, Camera* cam);

	void drawUI(App& game);
	void initSprites(SimpleTechnique& technique);
	Framebuffer& getFramebuffer();
	void update(float dt);
	void selectGameObject(GameObject* go);
	void unselectGameObject();
	void saveLevel(App& game);
	void loadLevel(App& game);
	void enable(bool value);
};