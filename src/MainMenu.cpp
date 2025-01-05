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
		GuiControlButton* bt = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, buttonName.c_str(), "", {0, 0, 0, 0}, this, {0,0,0,0});
		SetGuiParameters(bt, buttonName, buttonNode);
		buttons[buttonName] = bt;
		LOG("%s, %d", buttons[buttonName]->name, buttons[buttonName]->id);
	}
	

	pugi::xml_node sliderNode = configFile.child("config").child("mainmenu").child("optionsMenu").child("sliders");

	musicSlider = (GuiControlSlider*)Engine::Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::SLIDER, "musicSlider", "", { 0,0,0,0 }, this, { 0,0,0,0 });
	SetGuiParameters(musicSlider, "musicSlider", sliderNode);

	sfxSlider = (GuiControlSlider*)Engine::Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::SLIDER, "sfxSlider", "", { 0,0,0,0 }, this, { 0,0,0,0 });
	SetGuiParameters(sfxSlider, "sfxSlider", sliderNode);
	

	saved = rootNode.child("scene").child("savedData").attribute("saved").as_bool();
	LOG("%d", (int)saved);
	if (!saved)
		buttons["continueBt"]->state = GuiControlState::DISABLED();
	
	Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;
	

	Engine::GetInstance().audio.get()->PlayMusic(musicNode.child("menuMus").attribute("path").as_string(), 0.5f);
	Mix_VolumeMusic(MIX_MAX_VOLUME/5);

	bgTex = Engine::GetInstance().textures.get()->Load(configParameters.child("bg").attribute("path").as_string());
	///*pausePanel = Engine::GetInstance().textures.get()->Load(configParameters.child("bg").attribute("path").as_string());*/
	optPanel = Engine::GetInstance().textures.get()->Load(configParameters.child("optionsMenu").child("optPanel").attribute("path").as_string());
	optPanelX = configParameters.child("optionsMenu").child("optPanel").attribute("x").as_int();
	optPanelY = configParameters.child("optionsMenu").child("optPanel").attribute("y").as_int();
	

	//call it from scene-- wait you can't, it's disabled!
	//pause menu from scene
	//opt menu from main menu
	rootNode = configFile.child("config");
	
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
	
	
	if (settingsOpen) {
		/*OnGuiMouseClickEvent(musicSlider);*/
		Engine::GetInstance().render.get()->DrawTexture(optTex, 80, 80);
		/*musicSlider->active = true;
		musicSlider->Update(dt);*/
	}
	else {
		/*musicSlider->active = false;*/
	}
	

	if (quit) return false;
	
	
	return true;
}

// Update: draw background
bool MainMenu::PostUpdate()
{
	if (settingsOpen/* && pressed*/) {
		/*OnGuiMouseClickEvent(musicSlider);*/
		

		SDL_Rect windowRect = { 0,0,1280,720 };
		Engine::GetInstance().render.get()->DrawRectangle(windowRect, 0, 0, 0, 150, true, false);
		Engine::GetInstance().render.get()->DrawTexture(optPanel, optPanelX, optPanelY);
		
		for (const auto& bt : buttons)
		{
			if (bt.second->id != GuiControlId::OPTIONS && bt.second->id != GuiControlId::BACK)
				bt.second->state = GuiControlState::DISABLED;
			buttons["backBt"]->active = true;
			OnGuiMouseClickEvent(buttons["backBt"]);
			buttons["backBt"]->Update(_dt);
			
		}
		/*pressed = false;*/
		/*musicSlider->active = true;
		musicSlider->Update(dt);*/
	}
	else if(buttons["backBt"]->active) {
		buttons["backBt"]->active = false;
		for (const auto& bt : buttons)
		{
			if (bt.second->id != GuiControlId::BACK) {
				bt.second->state = GuiControlState::NORMAL;
			}
				
		}
		
		
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
			
		/*if (rootNode.child("scene").child("savedData").attribute("saved").as_bool() == true) 
			control->state == GuiControlState::DISABLED();*/
		
		break;
	case GuiControlId::CONTINUE:
		if (control->state == GuiControlState::PRESSED) {
			Engine::GetInstance().fade.get()->Fade((Module*)this, (Module*)Engine::GetInstance().scene.get(), 30);
			Engine::GetInstance().scene.get()->SetLoadState(true);
		}
		
		break;
	case GuiControlId::OPTIONS:

		if (control->state == GuiControlState::PRESSED) {
			if (!settingsOpen) settingsOpen = true;
			
		}
		
		break;
	case GuiControlId::BACK:
		if (control->state == GuiControlState::PRESSED) {
			if(settingsOpen) settingsOpen = false;
		}
	case GuiControlId::CREDITS:
			
		break;
	case GuiControlId::QUIT:
		if (control->state == GuiControlState::PRESSED) {
			quit = true;
		}
		break;
	}

	/*control->active = false;*/
	

	return true;
}

void MainMenu::SetGuiParameters(GuiControl* bt, std::string btName, pugi::xml_node parameters) {
	
	bt->id = (GuiControlId)parameters.child(btName.c_str()).attribute("id").as_int();
	if (bt->type == GuiControlType::SLIDER) {
		/*bt->texture = parameters.child()*/
		bt->bounds.x = parameters.child(btName.c_str()).attribute("circleX").as_int();
		bt->bounds.y = parameters.child(btName.c_str()).attribute("circleY").as_int();
		
	}
	else{

	}

	bt->bounds.x = parameters.child(btName.c_str()).attribute("x").as_int();
	bt->bounds.y = parameters.child(btName.c_str()).attribute("y").as_int();
	bt->bounds.w = parameters.child(btName.c_str()).attribute("w").as_int();
	bt->bounds.h = parameters.child(btName.c_str()).attribute("h").as_int();

	bt->texture = Engine::GetInstance().textures.get()->Load(parameters.child(btName.c_str()).attribute("texture").as_string());
}