#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Map.h"
#include "Pumpkin.h"
#include "Physics.h"
#include "BatEnemy.h"
#include "GroundEnemy.h"
#include <string>
#include "Particle.h"
#include "tracy/Tracy.hpp"
#include "GuiControl.h"
#include "GuiManager.h"
#include "GuiControlButton.h"
#include "Candy.h"
#include "MainMenu.h"
#include "FadeToBlack.h"
#include "Settings.h"

#include "UI.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	
	name = "scene";
	state = MAIN_MENU;
	/*active = false;*/
	
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	bool ret = true;
	/*if (!active) {
		return false;
	}*/
	LOG("Loading Scene");
	
	//L04: TODO 3b: Instantiate the player using the entity manager
	
	
	player->SetParameters(configParameters.child("entities").child("player"));
	Engine::GetInstance().map.get()->SetParameters(configParameters.child("scene").child("map"));

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	
	Engine::GetInstance().entityManager.get()->Enable();
	//player->Enable();
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	

	paused = false;

	
	//Load Map
	Engine::GetInstance().map->Load(configParameters.child("map").attribute("path").as_string(), configParameters.child("map").attribute("name").as_string());

	//Load Parallax
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax"));


	//Load HelpMenu
	help = false;
	helpPos.setX(configParameters.child("helpMenu").attribute("x").as_int());
	helpPos.setY(configParameters.child("helpMenu").attribute("y").as_int());
	helpMenu = Engine::GetInstance().textures.get()->Load(configParameters.child("helpMenu").attribute("path").as_string());

	//Load Bats

	Enemy* batEnemy2 = (BatEnemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::BAT_ENEMY);
	LoadEnemy(batEnemy2, configParameters.child("entities").child("enemies").child("flyEnemy").child("bat"), 3);

	Enemy* batEnemy3 = (BatEnemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::BAT_ENEMY);
	LoadEnemy(batEnemy3, configParameters.child("entities").child("enemies").child("flyEnemy").child("bat"), 5);

	/*Load Skeletons*/
	Enemy* groundEnemy = (GroundEnemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::GROUND_ENEMY);
	LoadEnemy(groundEnemy, configParameters.child("entities").child("enemies").child("groundEnemy").child("skeleton"), 2);

	Enemy* groundEnemy2 = (GroundEnemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::GROUND_ENEMY);
	LoadEnemy(groundEnemy2, configParameters.child("entities").child("enemies").child("groundEnemy").child("skeleton"), 4);

	Enemy* groundEnemy3 = (GroundEnemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::GROUND_ENEMY);
	LoadEnemy(groundEnemy3, configParameters.child("entities").child("enemies").child("groundEnemy").child("skeleton"), 6);

	

	//Load Pumpkins
	Pumpkin* pumpkin1 = (Pumpkin*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PUMPKIN);
	LoadItem(pumpkin1, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin1"));

	Pumpkin* checkPumpkin1 = (Pumpkin*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PUMPKIN);
	LoadItem(checkPumpkin1, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("checkPumpkin1"));

	Pumpkin* pumpkin2 = (Pumpkin*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PUMPKIN);
	LoadItem(pumpkin2, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin2"));

	Pumpkin* pumpkin3 = (Pumpkin*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PUMPKIN);
	LoadItem(pumpkin3, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin3"));

	Pumpkin* pumpkin4 = (Pumpkin*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PUMPKIN);
	LoadItem(pumpkin4, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin4"));

	Pumpkin* pumpkin5 = (Pumpkin*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PUMPKIN);
	LoadItem(pumpkin5, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin5"));

	Pumpkin* pumpkin6 = (Pumpkin*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PUMPKIN);
	LoadItem(pumpkin6, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin6"));

	Pumpkin* pumpkin7 = (Pumpkin*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PUMPKIN);
	LoadItem(pumpkin7, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin7"));

	Pumpkin* pumpkin8 = (Pumpkin*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PUMPKIN);
	LoadItem(pumpkin8, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin8"));

	//Load Items
	Candy* corn1 = (Candy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::CANDY);
	LoadItem(corn1, configParameters.child("entities").child("items").child("candies").child("instances").child("corn1"));

	Candy* swirl1 = (Candy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::CANDY);
	LoadItem(swirl1, configParameters.child("entities").child("items").child("candies").child("instances").child("swirl1"));

	Candy* heart1 = (Candy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::CANDY);
	LoadItem(heart1, configParameters.child("entities").child("items").child("candies").child("instances").child("heart1"));

	std::list<Entity*> entities = Engine::GetInstance().entityManager.get()->entities;
	for (const auto& entity : entities) {
		entity->Enable();
		entity->active = true;
		entity->renderable = true;
	}

	//Load PauseMenu
	pausePos.setX(configParameters.child("pauseMenu").child("pausePanel").attribute("x").as_int());
	pausePos.setY(configParameters.child("pauseMenu").child("pausePanel").attribute("y").as_int());
	pausePanel = Engine::GetInstance().textures.get()->Load(configParameters.child("pauseMenu").child("pausePanel").attribute("texture").as_string());
	
	pugi::xml_node pauseBtNode = configParameters.child("pauseMenu").child("buttons");
	
	pauseButtons.clear();
	for (pugi::xml_node child : pauseBtNode.children())
	{
		std::string buttonName = child.name();
		GuiControlButton* bt = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, buttonName.c_str(), "", { 0, 0, 0, 0 }, this, { 0,0,0,0 });
		this->SetGuiParameters(bt, buttonName, pauseBtNode);
		pauseButtons[buttonName] = bt;
		LOG("%s, %d", pauseButtons[buttonName]->name, pauseButtons[buttonName]->id);
		bt->active = false;
	}

	currentTime = 0;
	stoppedTimer = false;
	lvl1Timer.Start();

	return true;
}

void Scene::LoadEnemy(Enemy* enemy, pugi::xml_node parametersNode, int pathNum)
{
	enemy->SetPlayer(player);
	enemy->SetParameters(parametersNode);
	std::string nodeChar = "path" + std::to_string(pathNum);
	enemy->SetPath(configParameters.child("entities").child("enemies").child("paths").child(nodeChar.c_str()));
	enemies.push_back(enemy);
}

void Scene::LoadItem(Pumpkin* pumpkin, pugi::xml_node parametersNode) {

	pumpkin->SetPlayer(player);
	pumpkin->SetParameters(parametersNode);
	pumpkins.push_back(pumpkin);
}

void Scene::LoadItem(Candy* candy, pugi::xml_node parametersNode) {

	candy->SetPlayer(player);
	candy->SetParameters(parametersNode);
	candies.push_back(candy);
}

void Scene::RestartScene()
{
	player->Restart();

	for (int i = 0; i < enemies.size()/2; i++)
	{
		enemies[i]->Restart();
	}
}


// Called each loop iteration
bool Scene::PreUpdate()
{
	
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{	
	ZoneScoped;

	_dt = dt;

	if (loadScene)
	{
		//LOG("ENTRO");
		LoadState();
		loadScene = false;
	}

	//PlayMusic
	if (!musicPlays) {
		
		pugi::xml_document configFile;
		pugi::xml_parse_result result = configFile.load_file("config.xml");
		musicNode = configFile.child("config").child("audio").child("music");
		
		Engine::GetInstance().audio.get()->PlayMusic(musicNode.child("lvl1Mus").attribute("path").as_string());
		/*Mix_VolumeMusic(MIX_MAX_VOLUME / 5);*/
		
		musicPlays = true;
		
	}

	if (paused) {
		if (!stoppedTimer) {
			stoppedTimer = true;
			currentTime += lvl1Timer.ReadSec();
		}
		
	}
	else if(stoppedTimer) {
		stoppedTimer = false;
		lvl1Timer.Start();
	}
	
	
	if (!paused) {
		
	
		if (player->position.getX() < POS_TO_START_MOVING_CAMX) {
			Engine::GetInstance().render.get()->camera.x = (POS_TO_START_MOVING_CAMX + CAM_EXTRA_DISPLACEMENT_X) * -Engine::GetInstance().window.get()->scale;
		}
		else if (player->position.getX() > POS_TO_STOP_MOVING_CAMX) Engine::GetInstance().render.get()->camera.x = (POS_TO_STOP_MOVING_CAMX + CAM_EXTRA_DISPLACEMENT_X) * -Engine::GetInstance().window.get()->scale;
		else Engine::GetInstance().render.get()->camera.x = (player->position.getX() + CAM_EXTRA_DISPLACEMENT_X) * -Engine::GetInstance().window.get()->scale;

		//camera y
		if (player->position.getY() > POS_TO_START_MOVING_CAMY) {
			Engine::GetInstance().render.get()->camera.y = (POS_TO_START_MOVING_CAMY + CAM_EXTRA_DISPLACEMENT_Y) * -Engine::GetInstance().window.get()->scale;
		}
		else if (player->position.getY() < POS_TO_STOP_MOVING_CAMY) Engine::GetInstance().render.get()->camera.y = (POS_TO_STOP_MOVING_CAMY + CAM_EXTRA_DISPLACEMENT_X) * -Engine::GetInstance().window.get()->scale;
		else Engine::GetInstance().render.get()->camera.y = (player->position.getY() + CAM_EXTRA_DISPLACEMENT_Y) * -Engine::GetInstance().window.get()->scale;
	}



	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	
	bool ret = true;

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		SaveState();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		
		LoadState();
	}
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_H) == KEY_DOWN) {
		help = !help;
	}
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		paused = !paused;
	}

	Render* render = Engine::GetInstance().render.get();
	Window* window = Engine::GetInstance().window.get();

	
	
	

	if (!Engine::GetInstance().settings.get()->settingsOpen) {
		std::string timerText;
		std::string livesText = "Lives: " + std::to_string(player->lives);
		std::string ptsText = "Collected Candies: " + std::to_string(player->pickedCandies);
		if (paused) timerText = "Time: " + std::to_string((int)currentTime) + " s";
		else timerText = "Time: " + std::to_string((int)currentTime + (int)lvl1Timer.ReadSec()) + " s";
		render->DrawText(livesText.c_str(), 20, 20, 100, 20);
		render->DrawText(ptsText.c_str(), 150, 20, 200, 20);
		render->DrawText(timerText.c_str(), 375, 20, 100, 20);
	}

	if (paused && !Engine::GetInstance().settings.get()->settingsOpen) {

		
		Engine::GetInstance().render.get()->DrawRectangle({ -render->camera.x / window->scale , -render->camera.y / window->scale, window->width, window->height }, 0,0,0,200, true, true);
		Engine::GetInstance().render.get()->DrawTexture(pausePanel, -render->camera.x / window->scale + pausePos.getX(), -render->camera.y / window->scale + pausePos.getY());
		
		for (const auto& bt : pauseButtons) {
			if (bt.second->active == false) {
				bt.second->active = true;
			}	
			else {
				bt.second->Update(_dt);
				OnGuiMouseClickEvent(bt.second);
				
			}
			
		}

		if (Engine::GetInstance().settings.get()->settingsOpen) 
			for (const auto& bt : pauseButtons) 
				bt.second->state = GuiControlState::DISABLED;
		else 
			for (const auto& bt : pauseButtons) 
				bt.second->state = GuiControlState::NORMAL;
	}
	else {
		for (const auto& bt : pauseButtons) 
			bt.second->active = false;
		
	}

	if (help)
		render->DrawTexture(helpMenu, -render->camera.x / window->scale + helpPos.getX(), -render->camera.y / window->scale + helpPos.getY());

	if (quit) return false;
	
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	Engine::GetInstance().textures.get()->UnLoad(pausePanel);
	//Engine::GetInstance().entityManager.get()->DestroyEntity(player);
	

	for (const auto& bt : pauseButtons) {
		bt.second->active = false;
	}
	/*Mix_HaltMusic();*/
	/*player->Disable();*/
	/*for(const auto& entities)*/
	Engine::GetInstance().entityManager.get()->Disable();
	Engine::GetInstance().ui->Disable();
	

	LOG("Freeing scene");
	return true;
}

// Return the player position
Vector2D Scene::GetPlayerPosition()
{
	return player->GetPosition();
}

void Scene::SaveState()
{
	
	Engine::GetInstance().audio.get()->PlayFx(player->saveGameSFX);
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");

	saveFile.child("config").child("scene").child("savedData").attribute("saved").set_value(true);

	if (result == NULL)
	{
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node savedDataNode = saveFile.child("config").child("scene").child("savedData");

	//Save info to XML 
	//Player 
	player->SaveData(savedDataNode.child("player"));

	//Enemies
	for (int i = 0; i < enemies.size(); i++)
	{
		std::string nodeChar = "enemy" + std::to_string(i);
		pugi::xml_node parent = savedDataNode.child(nodeChar.c_str());

		if (!parent) {
			parent = savedDataNode.append_child(nodeChar.c_str());
			parent.append_attribute("dead");
			parent.append_attribute("x");
			parent.append_attribute("y");
		}

		enemies[i]->SaveData(parent);
	}

	//Pumpkins
	for (int i = 0; i < pumpkins.size(); i++)
	{
		std::string nodeChar = "pumpkin" + std::to_string(i);
		pugi::xml_node parent = savedDataNode.child(nodeChar.c_str());

		if (!parent) {
			parent = savedDataNode.append_child(nodeChar.c_str());
			parent.append_attribute("alight");
			parent.append_attribute("x");
			parent.append_attribute("y");
		}

		pumpkins[i]->SaveData(parent);
	}


	//Candies
	for (int i = 0; i < candies.size(); i++)
	{
		std::string nodeChar = "candy" + std::to_string(i);
		pugi::xml_node parent = savedDataNode.child(nodeChar.c_str());

		if (!parent) {
			parent = savedDataNode.append_child(nodeChar.c_str());
			parent.append_attribute("x");
			parent.append_attribute("y");
			parent.append_attribute("type");
			parent.append_attribute("picked");
		}

		candies[i]->SaveData(parent);
	}

	//Saves the modifications to the XML 
	saveFile.save_file("config.xml");
}

void Scene::LoadState() {

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (result == NULL) {
		LOG("Error loading config.xml");
		return;
	}

	pugi::xml_node savedDataNode = loadFile.child("config").child("scene").child("savedData");

	
	player->LoadData(savedDataNode.child("player"));

	bool enemyFound = true;
	for (int i = 0; enemyFound; i++)
	{
		std::string nodeChar = "enemy" + std::to_string(i);
		pugi::xml_node parent = savedDataNode.child(nodeChar.c_str());
		if (!parent)
		{
			enemyFound = false;
		}
		else
		{
			enemies[i]->LoadData(parent);
		}

	}

	loadFile.save_file("config.xml");

}

void Scene::SetLoadState(bool b)
{
	loadScene = b;
}


bool Scene::OnGuiMouseClickEvent(GuiControl* control) {

	switch (control->id) {
	case GuiControlId::RESUME:
		if (control->state == GuiControlState::PRESSED) {
			paused = false;
		}

		break;
	case GuiControlId::OPTIONS:
		if (control->state == GuiControlState::PRESSED) {
			if (!Engine::GetInstance().settings.get()->settingsOpen) {
				Engine::GetInstance().settings.get()->settingsOpen = true;
			}
		}
		break;

	case GuiControlId::BACKTOTITLE:
		if (control->state == GuiControlState::PRESSED) {
			
			Engine::GetInstance().fade.get()->Fade((Module*)this, (Module*)Engine::GetInstance().mainMenu.get(), 30);
			
			Engine::GetInstance().entityManager.get()->Disable();
			/*player->Disable();*/
			
		}
		break;
	case GuiControlId::QUIT:
		if (control->state == GuiControlState::PRESSED) {
			quit = true;
		}
		break;

	}

	

	return true;
}

void Scene::SetGuiParameters(GuiControl* bt, std::string btName, pugi::xml_node parameters) {

	bt->id = (GuiControlId)parameters.child(btName.c_str()).attribute("id").as_int();
	//if (bt->type == GuiControlType::SLIDER) {
	//	/*bt->texture = parameters.child()*/
	//	bt->bounds.x = parameters.child(btName.c_str()).attribute("circleX").as_int();
	//	bt->bounds.y = parameters.child(btName.c_str()).attribute("circleY").as_int();
	//}
	bt->bounds.x = parameters.child(btName.c_str()).attribute("x").as_int();
	bt->bounds.y = parameters.child(btName.c_str()).attribute("y").as_int();
	bt->bounds.w = parameters.child(btName.c_str()).attribute("w").as_int();
	bt->bounds.h = parameters.child(btName.c_str()).attribute("h").as_int();

	bt->texture = Engine::GetInstance().textures.get()->Load(parameters.child(btName.c_str()).attribute("texture").as_string());
}


