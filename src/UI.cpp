#include "UI.h"
#include "LOG.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Physics.h"


UI::UI() : Module()
{

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
	help = false;
	bool ret = true;
	pugi::xml_document configFile;
	pugi::xml_parse_result result = configFile.load_file("config.xml");
	helpMenu = Engine::GetInstance().textures.get()->Load(configFile.child("config").child("ui").attribute("path").as_string());


	if (helpMenu == nullptr)
	{
		LOG("Failed to load texture");
		ret = false;
	}

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
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		help = !help;
	}

	if (help)
	{
		Render* render = Engine::GetInstance().render.get();
		Window* window = Engine::GetInstance().window.get();
		render->DrawTexture(helpMenu,-render->camera.x/window->scale + HELP_MENU_X_DISPLACEMENT, -render->camera.y/ window->scale + HELP_MENU_Y_DISPLACEMENT);
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
	Engine::GetInstance().textures.get()->UnLoad(helpMenu);


	return true;
}
