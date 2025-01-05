#include "GuiControl.h"
#include "GuiManager.h"
#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "Settings.h"
#include "Engine.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "MainMenu.h"
#include "Scene.h"
#include "Window.h"


Settings::Settings(bool startEnabled) : Module(startEnabled)
{
	name = "settings";
}

// Destructor
Settings::~Settings()
{

}

// Called before render is available
bool Settings::Awake()
{
	LOG("Loading Settings Screen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Settings::Start()
{

	pugi::xml_parse_result result = configFile.load_file("config.xml");
	rootNode = configFile.child("config");

	musicSlider = (GuiControlSlider*)Engine::Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::SLIDER, "musicSlider", "", { 0,0,0,0 }, this, { 0,0,0,0 });
	SetGuiParameters(musicSlider, "musicSlider", configParameters.child("sliders"));
	settingsGUI.push_back(musicSlider);

	sfxSlider = (GuiControlSlider*)Engine::Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::SLIDER, "sfxSlider", "", { 0,0,0,0 }, this, { 0,0,0,0 });
	SetGuiParameters(sfxSlider, "sfxSlider", configParameters.child("sliders"));
	settingsGUI.push_back(sfxSlider);

	backBt = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, "backBt", "", { 0,0,0,0 }, this, { 0,0,0,0 });
	SetGuiParameters(backBt, "backBt", configParameters);
	settingsGUI.push_back(backBt);
	
	fullScreenBox = (GuiControlCheckBox*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::CHECKBOX, "fullScreenBox", "", { 0,0,0,0 }, this, { 0,0,0,0 });
	SetGuiParameters(fullScreenBox, "fullScreenBox", configParameters);
	settingsGUI.push_back(fullScreenBox);

	optPanel = Engine::GetInstance().textures.get()->Load(configParameters.child("optPanel").attribute("path").as_string());
	optPanelX = configParameters.child("optPanel").attribute("x").as_int();
	optPanelY = configParameters.child("optPanel").attribute("y").as_int();

	fullScreen = rootNode.child("window").child("fullscreen").attribute("value").as_bool();

	settingsOpen = false;
	return true;
}

// Called each loop iteration
bool Settings::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Settings::Update(float dt)
{
	SDL_Rect camera = Engine::GetInstance().render.get()->camera;
	int windowScale = Engine::GetInstance().window.get()->GetScale();
	if (settingsOpen){
		int screenWidth = rootNode.child("window").child("resolution").attribute("width").as_int();
		int screenHeight = rootNode.child("window").child("resolution").attribute("height").as_int();
		
		Engine::GetInstance().render.get()->DrawRectangle({ 0 , 0, screenWidth, screenHeight }, 0, 0, 0, 200, true, false);
		Engine::GetInstance().render.get()->DrawTexture(optPanel, -camera.x / windowScale + optPanelX, -camera.y / windowScale + optPanelY);

		
		for (GuiControl* gui : settingsGUI) {
			if(gui->active == false)
				gui->active = true;

		}
		OnGuiMouseClickEvent(backBt);
		backBt->Update(dt);
		fullScreenBox->Update(_dt);

		
		
		if (fullScreenBox->isChecked) {
			SDL_SetWindowFullscreen(Engine::GetInstance().window.get()->window, SDL_WINDOW_FULLSCREEN);
		}
		else {
			int windowW, windowH;
			Engine::GetInstance().window.get()->GetWindowSize(windowW, windowH);
			SDL_SetWindowFullscreen(Engine::GetInstance().window.get()->window, 0);
			SDL_SetWindowSize(Engine::GetInstance().window.get()->window, windowW, windowH);
		}
	}
	else {
		for (GuiControl* gui : settingsGUI) {
			gui->active = false;
		}
		
	}
	
	return true;
}

// Called each loop iteration
bool Settings::PostUpdate()
{
	
	
		
	
	return true;
}

// Called before quitting
bool Settings::CleanUp()
{
	LOG("Freeing UI");
	Engine::GetInstance().textures.get()->UnLoad(optPanel);

	for (GuiControl* gui : settingsGUI) {
		gui->active = false;
	}
	return true;
}

bool Settings::OnGuiMouseClickEvent(GuiControl* control) {

	switch (control->id) {
	case GuiControlId::MUSIC:
		break;
	case GuiControlId::SFX:
		break;
	case GuiControlId::FULLSCREEN:
		
		if (control->state == GuiControlState::PRESSED)  {
			
			
		}
		break;
	case GuiControlId::BACK:
		if (control->state == GuiControlState::PRESSED && settingsOpen) {
			settingsOpen = false;
			for (const auto& bt : Engine::GetInstance().mainMenu.get()->buttons) {
				bt.second->state = GuiControlState::NORMAL;
			}
			
		}
		
		break;
	}


	return true;
}


void Settings::SetGuiParameters(GuiControl* bt, std::string btName, pugi::xml_node parameters) {

	bt->id = (GuiControlId)parameters.child(btName.c_str()).attribute("id").as_int();
	if (bt->type == GuiControlType::SLIDER) {
		/*bt->texture = parameters.child()*/
		bt->bounds.x = parameters.child(btName.c_str()).attribute("circleX").as_int();
		bt->bounds.y = parameters.child(btName.c_str()).attribute("circleY").as_int();
	}
	else if (bt->type == GuiControlType::BUTTON || bt->type == GuiControlType::CHECKBOX) {
		bt->bounds.x = parameters.child(btName.c_str()).attribute("x").as_int();
		bt->bounds.y = parameters.child(btName.c_str()).attribute("y").as_int();
		bt->bounds.w = parameters.child(btName.c_str()).attribute("w").as_int();
		bt->bounds.h = parameters.child(btName.c_str()).attribute("h").as_int();
	}


	bt->texture = Engine::GetInstance().textures.get()->Load(parameters.child(btName.c_str()).attribute("texture").as_string());
}