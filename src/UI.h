#pragma once
#include "Module.h"
#include "Player.h"

#define HELP_MENU_X_DISPLACEMENT 125
#define HELP_MENU_Y_DISPLACEMENT 10

class UI : public Module
{
public:

	UI();

	// Destructor
	virtual ~UI();

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


	SDL_Texture* helpMenu;
	
	bool help;
	pugi::xml_node parameters;

};