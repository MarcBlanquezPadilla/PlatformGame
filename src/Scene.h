#pragma once

#include "Module.h"
#include "Player.h"

#define POS_TO_START_MOVING_CAMX 100
#define POS_TO_STOP_MOVING_CAMX 1050
#define CAM_EXTRA_DISPLACEMENT_X -100

#define POS_TO_START_MOVING_CAMY 198
#define POS_TO_STOP_MOVING_CAMY -200
#define CAM_EXTRA_DISPLACEMENT_Y -200

struct SDL_Texture;

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

private:
	SDL_Texture* img;

	//L03: TODO 3b: Declare a Player attribute
	Player* player;
};