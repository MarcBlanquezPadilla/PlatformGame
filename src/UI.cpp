#include "UI.h"
#include "LOG.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Scene.h"
#include "MainMenu.h"
#include "FadeToBlack.h"

UI::UI(bool startEnabled) : Module(startEnabled)
{
	name = "intro";
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
	intro = Engine::GetInstance().textures.get()->Load(configParameters.attribute("path").as_string());
	maxIntroTime = 5.0f;
	introTimer.Start();
	
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
	Engine::GetInstance().render.get()->DrawTexture(intro, 0, 0, NULL);
	if (introTimer.ReadSec() > maxIntroTime || (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)) {
		Engine::GetInstance().fade.get()->Fade(this, Engine::GetInstance().mainMenu.get(), 60);
	}

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
	Engine::GetInstance().textures.get()->UnLoad(intro);


	return true;
}
