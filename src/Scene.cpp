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
	//L06 TODO 3: Call the function to load the map. 
	
	
	//instead of passing both path and name strings directly, you pass the variable in config storing these values
	Engine::GetInstance().map->Load(configParameters.child("map").attribute("path").as_string(), configParameters.child("map").attribute("name").as_string());

	//Load Parallax -> TODO: Pass parallax to config
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax"));
	
	//Load Enemies
	batEnemy = (BatEnemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::BAT_ENEMY);
	batEnemy->SetPlayer(player);
	batEnemy->SetParameters(configParameters.child("entities").child("enemies").child("flyEnemy").child("bat"));
	groundEnemy = (GroundEnemy*)Engine::GetInstance().entityManager->CreateEntity(EntityType::GROUND_ENEMY);
	groundEnemy->SetPlayer(player);
	groundEnemy->SetParameters(configParameters.child("entities").child("enemies").child("groundEnemy").child("skeleton"));

	Item* pumpkin = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	/*pumpkin->SetPlayer(player);*/

	

	return true;
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

	pugi::xml_node sceneNode = saveFile.child("config").child("scene");

	//Save info to XML 

	//Player position
	sceneNode.child("entities").child("player").child("savedData").attribute("saved").set_value(true);
	sceneNode.child("entities").child("player").child("savedData").attribute("x").set_value(player->position.getX());
	sceneNode.child("entities").child("player").child("savedData").attribute("y").set_value(player->position.getY());
	sceneNode.child("entities").child("player").child("savedData").attribute("lives").set_value(player->lives);
	sceneNode.child("entities").child("player").child("savedData").attribute("transformed").set_value(player->transformed);
	sceneNode.child("entities").child("player").child("savedData").attribute("transformed").set_value(player->transformable);
	//enemies
	// ...

	//sceneNode.child("entities").child("player").attribute("x").set_value(player->position.getX());
	//sceneNode.child("entities").child("player").attribute("y").set_value(player->position.getY());

	//Saves the modifications to the XML 
	saveFile.save_file("config.xml");
}

void Scene::LoadState() {

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");
	if (result == NULL) {
		LOG("Error loading config.xml");
	}

	loadFile.child("config").child("scene").child("entities").child("player").child("savedData").attribute("saved").as_bool() == true;
	player->position.setX(loadFile.child("config").child("scene").child("entities").child("player").child("savedData").attribute("x").as_int());
	player->position.setY(loadFile.child("config").child("scene").child("entities").child("player").child("savedData").attribute("y").as_int());
	player->lives = loadFile.child("config").child("scene").child("entities").child("player").child("savedData").attribute("lives").as_int();
	player->transformed = loadFile.child("config").child("scene").child("entities").child("player").child("savedData").attribute("transformed").as_bool();
	player->transformable = loadFile.child("config").child("scene").child("entities").child("player").child("savedData").attribute("transformable").as_bool();
	//Load player position

	player->SetPosition(player->position);


}
// L15 TODO 2: Implement the Save function
