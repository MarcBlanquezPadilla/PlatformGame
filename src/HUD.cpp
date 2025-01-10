#include "HUD.h"
#include "LOG.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Scene.h"
#include "MainMenu.h"
#include "FadeToBlack.h"
#include "Scene.h"
#include "Settings.h"

HUD::HUD(bool startEnabled) : Module(startEnabled)
{
	name = "hud";
}

// Destructor
HUD::~HUD()
{

}

// Called before render is available
bool HUD::Awake()
{
	LOG("Loading UI");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool HUD::Start()
{
	
	
	return true;
}

// Called each loop iteration
bool HUD::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool HUD::Update(float dt)
{

	Render* render = Engine::GetInstance().render.get();
	Window* window = Engine::GetInstance().window.get();

	if (!Engine::GetInstance().settings.get()->settingsOpen) {
		std::string timerText;
		std::string livesText = "Lives: " + std::to_string(Engine::GetInstance().scene.get()->player->lives);
		std::string ptsText = "Collected Candies: " + std::to_string(Engine::GetInstance().scene.get()->GetPlayerCandies());

		if (Engine::GetInstance().scene.get()->paused) timerText = "Time: " + std::to_string((int)Engine::GetInstance().scene.get()->GetCurrentTime()) + " s";
		else timerText = "Time: " + std::to_string((int)Engine::GetInstance().scene.get()->GetCurrentTime()) + " s";
		render->DrawText(livesText.c_str(), 20, 20, 100, 20);
		render->DrawText(ptsText.c_str(), 150, 20, 200, 20);
		render->DrawText(timerText.c_str(), 375, 20, 100, 20);
	}

	return true;
}

// Called each loop iteration
bool HUD::PostUpdate()
{
	return true;
}

// Called before quitting
bool HUD::CleanUp()
{
	LOG("Freeing HUD");
	

	


	return true;
}
