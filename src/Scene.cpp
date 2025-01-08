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
#include "Santa.h"

#include "UI.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	
	name = "scene";
	player = nullptr;
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

	level = LVL1;
	
	//L04: TODO 3b: Instantiate the player using the entity manager	

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	paused = false;
	Engine::GetInstance().entityManager.get()->Enable();
	
	//Load HelpMenu
	help = false;
	helpPos.setX(configParameters.child("helpMenu").attribute("x").as_int());
	helpPos.setY(configParameters.child("helpMenu").attribute("y").as_int());
	helpMenu = Engine::GetInstance().textures.get()->Load(configParameters.child("helpMenu").attribute("path").as_string());

	if (loadScene) level = (Levels)configParameters.child("savedData").attribute("level").as_int();

	std::string path = configParameters.child("map").child("paths").child(GetLevelString().c_str()).attribute("path").as_string();
	std::string name = configParameters.child("map").child("paths").child(GetLevelString().c_str()).attribute("name").as_string();

	//Load Map
	Engine::GetInstance().map->Load(path, name);
	//Load Parallax
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax"));

	//Load Player
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	player->SetParameters(configParameters.child("entities").child("player"));

	//Load Enemies
	for (pugi::xml_node enemyNode : configParameters.child("entities").child("enemies").child("instances").child(GetLevelString().c_str()).children())
	{
		Enemy* enemy = (GroundEnemy*)Engine::GetInstance().entityManager->CreateEntity((EntityType)enemyNode.attribute("entityType").as_int());;
		LoadEnemy(enemy, enemyNode);
	}

	for (pugi::xml_node pumpkingNode : configParameters.child("entities").child("items").child("pumpkins").child("instances").child(GetLevelString().c_str()).children())
	{
		Pumpkin* pumpkin = (Pumpkin*)Engine::GetInstance().entityManager->CreateEntity((EntityType)pumpkingNode.attribute("entityType").as_int());;
		LoadItem(pumpkin, pumpkingNode);
	}

	for (pugi::xml_node candyNode : configParameters.child("entities").child("items").child("candies").child("instances").child(GetLevelString().c_str()).children())
	{
		Candy* candy = (Candy*)Engine::GetInstance().entityManager->CreateEntity((EntityType)candyNode.attribute("entityType").as_int());;
		LoadItem(candy, candyNode);
	}

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
		bt->active = false;
	}

	currentTime = 0;
	stoppedTimer = false;

	lvl1Timer.Start();

	if (!loadScene) SaveState();

	return true;
}

void Scene::LoadEnemy(Enemy* enemy, pugi::xml_node instanceNode)
{
	enemy->SetPlayer(player);
	enemy->SetParameters(configParameters.child("entities").child("enemies").child(instanceNode.attribute("enemyType").as_string()));
	enemy->SetInstanceParameters(instanceNode);
	enemy->SetPath(instanceNode);
	enemies.push_back(enemy);
}

void Scene::LoadItem(Pumpkin* pumpkin, pugi::xml_node instanceNode) {

	pumpkin->SetPlayer(player);
	pumpkin->SetParameters(configParameters.child("entities").child("items").child("pumpkins"));
	pumpkin->SetInstanceParameters(instanceNode);
	pumpkins.push_back(pumpkin);
}

void Scene::LoadItem(Candy* candy, pugi::xml_node instanceNode) {

	candy->SetPlayer(player);
	candy->SetParameters(configParameters.child("entities").child("items").child("candies"));
	candy->SetInstanceParameters(instanceNode);
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


int Scene::GetLevel()
{
	return (int)level;
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

	if (changeLevel)
	{
		changeLevel = false;
		Engine::GetInstance().fade.get()->Fade(this, this);
		return true;
	}

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
	Engine::GetInstance().map.get()->CleanUp();
	Engine::GetInstance().entityManager.get()->Disable();
	Engine::GetInstance().ui->Disable();
	Engine::GetInstance().physics.get()->DeleteAllPhysBody();

	if (player)
	{
		player->CleanUp();
		delete player;
		player = nullptr;
	}

	for (const auto& enemy : enemies) {
		enemy->CleanUp();
		delete enemy;
	}
	enemies.clear();

	for (const auto& candy : candies) {
		candy->CleanUp();
		delete candy;
	}
	candies.clear();

	for (const auto& pumpking : pumpkins) {
		pumpking->CleanUp();
		delete pumpking;
	}
	pumpkins.clear();

	for (const auto& bt : pauseButtons) {
		bt.second->active = false;
	}

	Mix_HaltMusic();
	
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
	
	pugi::xml_document saveFile;
	pugi::xml_parse_result result = saveFile.load_file("config.xml");

	saveFile.child("config").child("scene").child("savedData").attribute("saved").set_value(true);
	saveFile.child("config").child("scene").child("savedData").attribute("level").set_value((int)level);
	saveFile.child("config").child("scene").child("savedData").attribute("time").set_value(lvl1Timer.ReadSec());

	if (result == NULL)
	{
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node savedDataNode = saveFile.child("config").child("scene").child("savedData").child(GetLevelString().c_str());

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
	Engine::GetInstance().ReloadConfig();
}

void Scene::LoadState() {

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (result == NULL) {
		LOG("Error loading config.xml");
		return;
	}

	pugi::xml_node savedDataNode = loadFile.child("config").child("scene").child("savedData").child(GetLevelString().c_str());

	
	player->LoadData(savedDataNode.child("player"));


	//TODO: add an attribute to tell enemies from first and second level apart
	bool enemyFound = true;
	for (int i = 0; i<enemies.size() || enemyFound; i++)
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

void Scene::ChangeLevel()
{
	level = LVL2;
	changeLevel = true;
}

std::string Scene::GetLevelString()
{
	return "lvl" + std::to_string((int)level);
}

void Scene::SetLevel(Levels level)
{
	level = level;
}

bool Scene::ReloadParameters(pugi::xml_node parameters)
{
	LoadParameters(parameters);
	if (player)
	{
		player->SetParameters(configParameters.child("entities").child("player"));
	}
	return true;
}

