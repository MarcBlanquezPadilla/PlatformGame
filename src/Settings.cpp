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

	testSound = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/Fushigi.ogg");

	pugi::xml_parse_result result = configFile.load_file("config.xml");
	rootNode = configFile.child("config");


	SDL_Texture* circleTex = Engine::GetInstance().textures.get()->Load(configParameters.child("sliders").attribute("texture").as_string());
	SDL_Texture* barTex = Engine::GetInstance().textures.get()->Load(configParameters.child("sliders").attribute("barTexture").as_string());


	musicSlider = (GuiControlSlider*)Engine::Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::SLIDER, "musicSlider", "", { 0,0,0,0 }, this, { 0,0,0,0 }, circleTex, barTex);
	/*musicSlider->sliderTexture = barTex;*/
	SetGuiParameters(musicSlider, "musicSlider", configParameters.child("sliders"));
	settingsGUI.push_back(musicSlider);

	sfxSlider = (GuiControlSlider*)Engine::Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::SLIDER, "sfxSlider", "", { 0,0,0,0 }, this, { 0,0,0,0 }, circleTex, barTex);
	SetGuiParameters(sfxSlider, "sfxSlider", configParameters.child("sliders"));
	/*sfxSlider->sliderTexture = barTex;*/
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

	/*fullScreen = rootNode.child("window").child("fullscreen").attribute("value").as_bool();*/
	/*musicSlider->sliderPosX = musicSlider->sliderBounds.x + musicSlider->sliderBounds.w - 10;
	sfxSlider->sliderPosX = sfxSlider->sliderBounds.x + sfxSlider->sliderBounds.w - 10;*/
	
	musicSlider->sliderPosX = musicSlider->sliderBounds.x + musicSlider->sliderBounds.w/2 - musicSlider->bounds.w/2;
	sfxSlider->sliderPosX = sfxSlider->sliderBounds.x + sfxSlider->sliderBounds.w/2 - sfxSlider->bounds.w/2;

	LoadPrefs();

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
			if (gui->active == false) {
				gui->active = true;
			}
		}
		
		backBt->Update(dt);
		OnGuiMouseClickEvent(backBt);

		fullScreenBox->Update(dt);

		
		musicSlider->Update(dt);
		OnGuiMouseClickEvent(musicSlider);
		
		sfxSlider->Update(dt);
		/*OnGuiMouseClickEvent(sfxSlider);*/


	
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

	for (GuiControl* gui : settingsGUI) {
		gui->active = false;
	}
	return true;
}

bool Settings::OnGuiMouseClickEvent(GuiControl* control) {

	switch (control->id) {
	case GuiControlId::MUSIC:
		musicVolume = SetVolume((GuiControlSlider*)control);
		Mix_VolumeMusic(musicVolume);
		break;
	case GuiControlId::SFX:
		/*if(state == GuiControlState)*/
		sfxVolume = SetVolume((GuiControlSlider*)control);
		Engine::GetInstance().audio.get()->PlayFx(testSound);
		break;
	case GuiControlId::FULLSCREEN:
		break;
	case GuiControlId::BACK:
		if (control->state == GuiControlState::PRESSED && settingsOpen) {
			settingsOpen = false;
			for (const auto& bt : Engine::GetInstance().mainMenu.get()->buttons) {
				bt.second->state = GuiControlState::NORMAL;
			}
			SavePrefs();
		}
		
		break;
	}


	return true;
}


void Settings::SetGuiParameters(GuiControl* bt, std::string btName, pugi::xml_node parameters) {

	bt->id = (GuiControlId)parameters.child(btName.c_str()).attribute("id").as_int();
	if (bt->type == GuiControlType::SLIDER) {
		bt->sliderBounds.x = parameters.child(btName.c_str()).attribute("barX").as_int();
		bt->sliderBounds.y = parameters.child(btName.c_str()).attribute("barY").as_int();
		bt->sliderBounds.w = parameters.attribute("barW").as_int();
		bt->sliderBounds.h = parameters.attribute("barH").as_int();
		bt->bounds.x = parameters.child(btName.c_str()).attribute("circleX").as_int();
		bt->bounds.y = parameters.child(btName.c_str()).attribute("circleY").as_int();
		bt->bounds.w = parameters.attribute("circleW").as_int();
		bt->bounds.h = parameters.attribute("circleH").as_int();
	}
	else if (bt->type == GuiControlType::BUTTON || bt->type == GuiControlType::CHECKBOX) {
		bt->bounds.x = parameters.child(btName.c_str()).attribute("x").as_int();
		bt->bounds.y = parameters.child(btName.c_str()).attribute("y").as_int();
		bt->bounds.w = parameters.child(btName.c_str()).attribute("w").as_int();
		bt->bounds.h = parameters.child(btName.c_str()).attribute("h").as_int();
		bt->texture = Engine::GetInstance().textures.get()->Load(parameters.child(btName.c_str()).attribute("texture").as_string());
	}


	
}

int Settings::SetVolume(GuiControlSlider* slider) {

	return (slider->volumeValue);
}

void Settings::SavePrefs()
{
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");

	saveFile.child("config").child("playerPrefs").child("fullscreen").attribute("toggle").set_value((int)fullScreenBox->isChecked);
	saveFile.child("config").child("playerPrefs").child("musicVolume").attribute("value").set_value(musicVolume);
	saveFile.child("config").child("playerPrefs").child("sfxVolume").attribute("value").set_value(sfxVolume);

	//Saves the modifications to the XML 
	saveFile.save_file("config.xml");
	Engine::GetInstance().ReloadConfig();
}

void Settings::LoadPrefs()
{
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");

	fullScreen = saveFile.child("config").child("playerPrefs").child("fullscreen").attribute("toggle").as_bool();
	musicVolume = saveFile.child("config").child("playerPrefs").child("musicVolume").attribute("value").as_int();
	sfxVolume = saveFile.child("config").child("playerPrefs").child("sfxVolume").attribute("value").as_int();

	fullScreenBox->SetChecked(fullScreen);
	if (fullScreenBox->isChecked) {
		SDL_SetWindowFullscreen(Engine::GetInstance().window.get()->window, SDL_WINDOW_FULLSCREEN);
	}
	musicSlider->SetVolumeValue(musicVolume);
	sfxSlider->SetVolumeValue(sfxVolume);
}