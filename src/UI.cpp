#include "UI.h"
#include "LOG.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"
#include "Scene.h"


UI::UI(bool startEnabled) : Module(startEnabled)
{
	name = "UI";
}

// Destructor
UI::~UI()
{

}

// Called before render is available
bool UI::Awake()
{
	LOG("Loading UI");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool UI::Start()
{
	

	

	/*if (helpMenu == nullptr)
	{
		LOG("Failed to load texture");
		ret = false;
	}*/

	return true;
}

// Called each loop iteration
bool UI::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool UI::Update(float dt)
{
	
	
	
	

	return true;
}

// Called each loop iteration
bool UI::PostUpdate()
{
	return true;
}

// Called before quitting
bool UI::CleanUp()
{
	LOG("Freeing UI");
	Engine::GetInstance().textures.get()->UnLoad(helpMenu);


	return true;
}
