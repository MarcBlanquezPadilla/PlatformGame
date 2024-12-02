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
#include "Item.h"
#include "Physics.h"
#include "BatEnemy.h"
#include "GroundEnemy.h"
#include <string>

Scene::Scene() : Module()
{
	name = "scene";
	
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	//L04: TODO 3b: Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER);
	player->SetParameters(configParameters.child("entities").child("player"));
	
	Engine::GetInstance().map.get()->SetParameters(configParameters.child("scene").child("map"));

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//Load map. 
	Engine::GetInstance().map->Load(configParameters.child("map").attribute("path").as_string(), configParameters.child("map").attribute("name").as_string());

	//Load Parallax
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax"));

	//Load Enemies
	Enemy* batEnemy = (BatEnemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::BAT_ENEMY);
	LoadEnemy(batEnemy, configParameters.child("entities").child("enemies").child("flyEnemy").child("bat"), 1);

	Enemy* groundEnemy = (GroundEnemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::GROUND_ENEMY);
	LoadEnemy(groundEnemy, configParameters.child("entities").child("enemies").child("groundEnemy").child("skeleton"), 2);

	//Load Items
	Item* pumpkin = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);

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


// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{	


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

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		Engine::GetInstance().audio.get()->PlayFx(player->saveGame);
		LOG("Played save game sound fx");
		SaveState();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		LoadState();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
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
			parent.append_attribute("alive");
			parent.append_attribute("x");
			parent.append_attribute("y");
		}

		enemies[i]->SaveData(parent);
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
// L15 TODO 2: Implement the Save function
