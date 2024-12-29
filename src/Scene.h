#pragma once

#include "Module.h"
#include "Player.h"
#include <vector>

#define POS_TO_START_MOVING_CAMX 100
#define POS_TO_STOP_MOVING_CAMX 2180
#define CAM_EXTRA_DISPLACEMENT_X -100

#define POS_TO_START_MOVING_CAMY 248
#define POS_TO_STOP_MOVING_CAMY -190
#define CAM_EXTRA_DISPLACEMENT_Y -100

struct SDL_Texture;

class BatEnemy;
class GroundEnemy;
class Enemy;
class Pumpkin;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Return the player position
	Vector2D GetPlayerPosition();

	void LoadEnemy(Enemy* enemy, pugi::xml_node parametersNode, int pathNum);

	void RestartScene();

	void LoadItem(Pumpkin* pumpkins, pugi::xml_node parametersNode);

	void LoadState();

	void SaveState();

private:
	
	//L03: TODO 3b: Declare a Player attribute
	Player* player;

	std::vector<Enemy*> enemies;
	std::vector<Pumpkin*> pumpkins;


};
