#include "MainMenu.h"

#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"
#include "EntityManager.h"

MainMenu::MainMenu(bool startEnabled) : Module(startEnabled)
{

}

MainMenu::~MainMenu()
{

}

// Load assets
bool MainMenu::Start()
{
	LOG("Loading background assets");

	bool ret = true;
	configFile.load_file("config.xml");
	
	pugi::xml_parse_result result = configFile.load_file("config.xml");
	pugi::xml_node musicNode = configFile.child("config").child("audio").child("music");
	

	Engine::GetInstance().audio.get()->PlayMusic(musicNode.child("menuMus").attribute("path").as_string());

	bgTex = Engine::GetInstance().textures.get()->Load("Assets/Textures/Menus/Title Screen/title screen model 2.png");
	

	Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;

	return ret;
}

bool MainMenu::Update(float dt)
{
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		Engine::GetInstance().fade.get()->Fade((Module*)this, (Module*)Engine::GetInstance().scene.get(), 90);
		/*Engine::GetInstance().fade.get()->Fade((Module*)this, (Module*)Engine::GetInstance().entityManager.get(), 90);*/
	}

	return true;
}

// Update: draw background
bool MainMenu::PostUpdate()
{
	// Draw everything --------------------------------------
	Engine::GetInstance().render.get()->DrawTexture(bgTex, 0, 0, NULL);

	return true;
}