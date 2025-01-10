#pragma once
#include "Module.h"
#include "Timer.h"
#include "Animation.h"

struct SDL_Texture;

class HUD : public Module
{
public:

	HUD(bool startEnabled);

	// Destructor
	virtual ~HUD();

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
	SDL_Texture* lifeContainer, *candyIcon, *timerIcon;
	Animation full, empty, loseLife;

	/*pugi::xml_node parameters;*/

};