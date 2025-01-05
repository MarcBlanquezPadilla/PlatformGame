#pragma once

#include "Module.h"
#include "Player.h"
#include <vector>
#include "Audio.h"
#include "SDL2/SDL_mixer.h"
#include "GuiControl.h"
#include <map>
#include <list>


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
class GuiControlButton;
class GuiControlSlider;
class GuiControlCheckBox;


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

	bool OnGuiMouseClickEvent(GuiControl* control);

	void SetGuiParameters(GuiControl* bt, std::string btName, pugi::xml_node parameters);

public:
	GameState state;
	bool musicPlays = false;
	bool paused = false;
	bool help = false;
	Player* player;
	Timer lvl1Timer;
	float lvl1Volume;

	float currentTime;
	bool stoppedTimer;

	SDL_Texture* helpMenu;

	GuiControlButton* resumeBt, * settingsBt, * backToTitleBt, * exitBt, *backBt;

	GuiControlCheckBox* fullScreenCheckBox;
	SDL_Texture* pausePanel;
	Vector2D pausePos;
	

private:
	
	//L03: TODO 3b: Declare a Player attribute
	float _dt;

	std::vector<Enemy*> enemies;
	std::vector<Pumpkin*> pumpkins;
	std::vector<Candy*> candies;
	pugi::xml_node musicNode;

	std::map<std::string, GuiControlButton*> pauseButtons;
	
	bool loadScene = false;
	bool quit = false;

	Vector2D helpPos;

	
};
