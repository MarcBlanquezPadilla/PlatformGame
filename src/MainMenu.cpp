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
	pugi::xml_node musicNode = configFile.child("config").child("audio").child("music");

	btTex = Engine::GetInstance().textures.get()->Load(configParameters.child("buttons").attribute("defaultTex").as_string());
	int texW, texH;
	Engine::GetInstance().textures.get()->GetSize(btTex, texW, texH);

	btFont = TTF_OpenFont("Assets/Fonts/Corvid Conspirator v1.1.ttf", 30);

	pugi::xml_node buttonNode = configFile.child("config").child("mainmenu").child("buttons");

	btList.push_back(newGameBt);
	btList.push_back(continueBt);
	btList.push_back(optionsBt);
	btList.push_back(creditsBt);
	btList.push_back(exitBt);

	int i = 1;
	pugi::xml_node child = buttonNode.first_child();
	for (auto& bt : btList) {
		bt = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, i, "", {0, 0, 0, 0}, this, {0,0,0,0});
		std::string buttonName = child.name();
		SetButtonParameters(bt, buttonName, buttonNode);
		i++;
		child = child.next_sibling();
	}
	
	
	Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;
	

	Engine::GetInstance().audio.get()->PlayMusic(musicNode.child("menuMus").attribute("path").as_string(), 0.5f);
	Mix_VolumeMusic(MIX_MAX_VOLUME/5);

	bgTex = Engine::GetInstance().textures.get()->Load(configParameters.child("bg").attribute("path").as_string());
	
	rootNode = configFile.child("config");

	return ret;
}

bool MainMenu::Update(float dt)
{
	
	Engine::GetInstance().render.get()->DrawTexture(bgTex, 0, 0, NULL);
	for (const auto& bt : btList)
	{
		
		OnGuiMouseClickEvent(bt);
		bt->Update(dt);
	}

	//Engine::GetInstance().scene.get()->player->SetPosition({ 640 / 2, 320 / 2 + 75});
	//Engine::GetInstance().scene.get()->player->playerState = IDLE;
	///*Engine::GetInstance().scene.get()->player->texW *= 2;
	//Engine::GetInstance().scene.get()->player->texH *= 2;*/
	//Engine::GetInstance().scene.get()->player->Update(dt);
	
	
	if (quit) return false;
	
	
	return true;
}

// Update: draw background
bool MainMenu::PostUpdate()
{
	return true;
}

bool MainMenu::CleanUp() {
	/*newGameBt->active = false;
	continueBt->active = false;
	creditsBt->active = false;
	optionsBt->active = false;
	exitBt->active = false;*/
	for (const auto& bt : btList) {
		bt->active = false;
	}
	Engine::GetInstance().textures.get()->UnLoad(btTex);
	return true;
}

bool MainMenu::OnGuiMouseClickEvent(GuiControl* control) {
	
	saved = rootNode.child("scene").child("savedData").attribute("saved").as_bool();
	switch (control->id) {
	case 1:
		if(control->state == GuiControlState::PRESSED){
			Engine::GetInstance().fade.get()->Fade((Module*)this, (Module*)Engine::GetInstance().scene.get(), 30);
			rootNode.child("scene").child("savedData").attribute("saved").set_value(false);
		}
			
		/*if (rootNode.child("scene").child("savedData").attribute("saved").as_bool() == true) 
			control->state == GuiControlState::DISABLED();*/
		
		break;
	case 2:
		if (!saved) 
			control->state = GuiControlState::DISABLED();
		else if (control->state == GuiControlState::PRESSED) {
			Engine::GetInstance().fade.get()->Fade((Module*)this, (Module*)Engine::GetInstance().scene.get(), 30);
			/*rootNode.child("scene").child("savedData").attribute("saved").set_value(true);*/
			/*Engine::GetInstance().scene.get()->player->LoadData(Engine::GetInstance().scene.get()->configParameters.child("savedData").child("player"));*/
			/*Engine::GetInstance().scene.get()->player->SetParameters(configFile.child("scene").child("savedData").child("player"));*/
		}
			
		
		break;
	case 3:
		
		break;
	case 4:
			
		break;
	case 5:
		if (control->state == GuiControlState::PRESSED) {
			quit = true;
		}
		break;
	}

	/*control->active = false;*/
	

	return true;
}

void MainMenu::SetButtonParameters(GuiControlButton* bt, std::string btName, pugi::xml_node buttonParameters) {
	
	
	bt->bounds.x = buttonParameters.child(btName.c_str()).attribute("x").as_int();
	bt->bounds.y = buttonParameters.child(btName.c_str()).attribute("y").as_int();
	bt->bounds.w = buttonParameters.child(btName.c_str()).attribute("w").as_int();
	bt->bounds.h = buttonParameters.child(btName.c_str()).attribute("h").as_int();

	bt->texture = Engine::GetInstance().textures.get()->Load(buttonParameters.child(btName.c_str()).attribute("texture").as_string());
}