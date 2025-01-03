#pragma once

#include "Module.h"
#include "Player.h"
#include <vector>
#include "Audio.h"
#include "SDL2/SDL_mixer.h"

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
class Candy;
class EntityManager;


enum GameState {
	MAIN_MENU,
	LVL1,
	LVL2,
	WIN_SCREEN,
	LOSE_SCREEN
};

class Scene : public Module
{
public:

	Scene(bool startEnabled);

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

	void LoadItem(Candy* candy, pugi::xml_node parametersNode);

	void LoadState();

	void SaveState();

	void SetLoadState(bool load);

public:
	GameState state;
	bool musicPlays = false;
	Player* player;
	Timer lvl1Timer;

private:
	
	//L03: TODO 3b: Declare a Player attribute
	
	
	std::vector<Enemy*> enemies;
	std::vector<Pumpkin*> pumpkins;
	std::vector<Candy*> candies;
	pugi::xml_node musicNode;
	
	bool loadScene = false;
};
