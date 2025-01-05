#include "MainMenu.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Audio.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "GuiManager.h"
#include "EntityManager.h"
#include "Scene.h"
#include "Settings.h"
#include "Player.h"




MainMenu::MainMenu(bool startEnabled) : Module(startEnabled)
{
	name = "mainmenu";
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
	rootNode = configFile.child("config");	
	pugi::xml_node musicNode = rootNode.child("audio").child("music");

	btTex = Engine::GetInstance().textures.get()->Load(configParameters.child("buttons").attribute("defaultTex").as_string());
	int texW, texH;
	Engine::GetInstance().textures.get()->GetSize(btTex, texW, texH);

	btFont = TTF_OpenFont("Assets/Fonts/Corvid Conspirator v1.1.ttf", 30);

	pugi::xml_node buttonNode = configFile.child("config").child("mainmenu").child("buttons");

	buttons.clear();
	for (pugi::xml_node child : buttonNode.children())
	{
		std::string buttonName = child.name();
		GuiControlButton* bt = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, buttonName.c_str(), "", { 0, 0, 0, 0 }, this, { 0,0,0,0 });
		SetGuiParameters(bt, buttonName, buttonNode);
		buttons[buttonName] = bt;
		LOG("%s, %d", buttons[buttonName]->name, buttons[buttonName]->id);
	}
	
	Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;
	

	Engine::GetInstance().audio.get()->PlayMusic(musicNode.child("menuMus").attribute("path").as_string(), 0.5f);
	Mix_VolumeMusic(MIX_MAX_VOLUME/5);

	bgTex = Engine::GetInstance().textures.get()->Load(configParameters.child("bg").attribute("path").as_string());
	///*pausePanel = Engine::GetInstance().textures.get()->Load(configParameters.child("bg").attribute("path").as_string());*/
	
	rootNode = configFile.child("config");

	saved = rootNode.child("scene").child("savedData").attribute("saved").as_bool();
	LOG("%d", (int)saved);
	if (!saved)
		buttons["continueBt"]->state = GuiControlState::DISABLED();

	return ret;
}

bool MainMenu::Update(float dt)
{
	_dt = dt;

	Engine::GetInstance().render.get()->DrawTexture(bgTex, 0, 0, NULL);
	for (const auto& bt : buttons)
	{
		OnGuiMouseClickEvent(bt.second);
		/*if(bt.second->id != GuiControlId::BACK)*/
		bt.second->Update(dt);
	}
	
	

	if (quit) return false;
	
	
	return true;
}

// Update: draw background
bool MainMenu::PostUpdate()
{
	if (Engine::GetInstance().settings.get()->settingsOpen/* && pressed*/) {
		/*OnGuiMouseClickEvent(musicSlider);*/
		for (const auto& bt : buttons)
			bt.second->state = GuiControlState::DISABLED;
	}

	return true;
}

bool MainMenu::CleanUp() {

	for (const auto& bt : buttons) {
		bt.second->active = false;
	}
	/*musicSlider->active = false;*/
	Engine::GetInstance().textures.get()->UnLoad(btTex);
	Engine::GetInstance().textures.get()->UnLoad(optPanel);
	Engine::GetInstance().textures.get()->UnLoad(pausePanel);

	return true;
}

bool MainMenu::OnGuiMouseClickEvent(GuiControl* control) {
	
	saved = rootNode.child("scene").child("savedData").attribute("saved").as_bool();
	switch (control->id) {
	case GuiControlId::NEW_GAME:
		if(control->state == GuiControlState::PRESSED){
			Engine::GetInstance().fade.get()->Fade((Module*)this, (Module*)Engine::GetInstance().scene.get(), 30);
			rootNode.child("scene").child("savedData").attribute("saved").set_value(false);
		}
		
		break;
	case GuiControlId::CONTINUE:
		if (control->state == GuiControlState::PRESSED) {
			Engine::GetInstance().fade.get()->Fade((Module*)this, (Module*)Engine::GetInstance().scene.get(), 30);
			Engine::GetInstance().scene.get()->SetLoadState(true);
		}
		
		break;
	case GuiControlId::OPTIONS:
		if (control->state == GuiControlState::PRESSED) {
			if (!Engine::GetInstance().settings.get()->settingsOpen) 
				Engine::GetInstance().settings.get()->settingsOpen = true;
		}
		break;
	/*case GuiControlId::BACK:
		if (control->state == GuiControlState::PRESSED) {
			if (settingsOpen) {
				settingsOpen = false;
				for (const auto& bt : buttons) {
					bt.second->state = GuiControlState::NORMAL;
				}
			}
		}*/
	case GuiControlId::CREDITS:
			
		break;
	case GuiControlId::QUIT:
		if (control->state == GuiControlState::PRESSED) {
			quit = true;
		}
		break;

	}
	
	return true;
}

void MainMenu::SetGuiParameters(GuiControl* bt, std::string btName, pugi::xml_node parameters) {
	
	bt->id = (GuiControlId)parameters.child(btName.c_str()).attribute("id").as_int();
	if (bt->type == GuiControlType::SLIDER) {
		/*bt->texture = parameters.child()*/
		bt->bounds.x = parameters.child(btName.c_str()).attribute("circleX").as_int();
		bt->bounds.y = parameters.child(btName.c_str()).attribute("circleY").as_int();
	}

	bt->bounds.x = parameters.child(btName.c_str()).attribute("x").as_int();
	bt->bounds.y = parameters.child(btName.c_str()).attribute("y").as_int();
	bt->bounds.w = parameters.child(btName.c_str()).attribute("w").as_int();
	bt->bounds.h = parameters.child(btName.c_str()).attribute("h").as_int();

	bt->texture = Engine::GetInstance().textures.get()->Load(parameters.child(btName.c_str()).attribute("texture").as_string());
}
