#pragma once

#include "Module.h"
#include "Animation.h"
//#include "GuiControl.h"
#include "GuiControlButton.h"
#include <list>
#include <map>


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

	bool CleanUp() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	bool PostUpdate() override;

	bool OnGuiMouseClickEvent(GuiControl* control);

	void SetButtonParameters(GuiControlButton* bt, std::string btName, pugi::xml_node buttonParameters);

public:
	// The scene sprite sheet loaded into an SDL_Texture
	pugi::xml_document configFile;
	pugi::xml_node rootNode;
	SDL_Texture* bgTex, * btTex;
	TTF_Font* btFont;
	bool quit = false;
	bool saved;

	std::map<std::string, GuiControlButton*> buttons;

};