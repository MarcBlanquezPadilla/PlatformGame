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
#include "Particle.h"

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


	//Load Items
	Item* pumpkin1 = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	LoadItem(pumpkin1, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin1"));

	Item* checkPumpkin1 = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	LoadItem(checkPumpkin1, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("checkPumpkin1"));

	Item* pumpkin2 = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	LoadItem(pumpkin2, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin2"));

	Item* pumpkin3 = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	LoadItem(pumpkin3, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin3"));

	Item* pumpkin4 = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	LoadItem(pumpkin4, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin4"));

	Item* pumpkin5 = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	LoadItem(pumpkin5, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin5"));

	Item* pumpkin6 = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	LoadItem(pumpkin6, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin6"));

	Item* pumpkin7 = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	LoadItem(pumpkin7, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin7"));

	Item* pumpkin8 = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	LoadItem(pumpkin8, configParameters.child("entities").child("items").child("pumpkins").child("instances").child("pumpkin8"));


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

void Scene::LoadItem(Item* item, pugi::xml_node parametersNode) {

	item->SetPlayer(player);
	item->SetParameters(parametersNode);
	items.push_back(item);
}

void Scene::RestartScene()
{
	player->Restart();

	for (int i = 0; i < enemies.size(); i++)
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
	
	
	Engine::GetInstance().audio.get()->PlayFx(player->saveGame);
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

	//Items
	for (int i = 0; i < items.size(); i++)
	{
		std::string nodeChar = "item" + std::to_string(i);
		pugi::xml_node parent = savedDataNode.child(nodeChar.c_str());

		if (!parent) {
			parent = savedDataNode.append_child(nodeChar.c_str());
			parent.append_attribute("alight");
			parent.append_attribute("x");
			parent.append_attribute("y");
		}

		items[i]->SaveData(parent);
	}

	//Saves the modifications to the XML 
	saveFile.save_file("config.xml");
}

void Scene::LoadState() {

	Engine::GetInstance().audio.get()->PlayFx(player->loadGame);

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
