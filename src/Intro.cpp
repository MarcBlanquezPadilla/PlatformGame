#include "GuiControl.h"
#include "GuiManager.h"
#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "Intro.h"
#include "Engine.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "MainMenu.h"
#include "Scene.h"
#include "Window.h"


Intro::Intro(bool startEnabled) : Module(startEnabled)
{
	name = "intro";
}

// Destructor
Intro::~Intro()
{

}

// Called before render is available
bool Intro::Awake()
{
	LOG("Loading Intro Screen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Intro::Start()
{





	return true;
}

// Called each loop iteration
bool Intro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Intro::Update(float dt)
{
	

	return true;
}

// Called each loop iteration
bool Intro::PostUpdate()
{

	return true;
}

// Called before quitting
bool Intro::CleanUp()
{
	LOG("Freeing UI");
	
	return true;
}

