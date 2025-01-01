#pragma once

#include "Module.h"
#include "Animation.h"

struct SDL_Texture;

class MainMenu : public Module
{
public:
	//Constructor
	MainMenu(bool startEnabled);

	//Destructor
	~MainMenu();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	bool Update(float dt) override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTex = nullptr;
	pugi::xml_document configFile;
};