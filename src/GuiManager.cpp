#include "GuiManager.h"
#include "Engine.h"
#include "Textures.h"

#include "GuiControlButton.h"
#include "Audio.h"

GuiManager::GuiManager(bool startEnabled) : Module(startEnabled)
{
	name = "guiManager";
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}


// L16: TODO 1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, const char* name,const char* text, SDL_Rect bounds, Module* _observer, SDL_Rect sliderBounds, SDL_Texture* tex)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(name, bounds, text, tex);
		break;
	}

	//Set the observer
	guiControl->observer = _observer;

	guiControl->texture = tex;

	// Created GuiControls are add it to the list of controls
	guiControlsList.push_back(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{
	for (const auto& control : guiControlsList)
	{
		control->Update(dt);
	}

	return true;
}

bool GuiManager::CleanUp()
{
	for (const auto& control : guiControlsList)
	{
		delete control;
	}

	return true;
}