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

Scene::Scene() : Module()
{
	name = "scene";
	img = nullptr;
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
	Engine::GetInstance().map.get()->SetParameters(configParameters.child("entities").child("map"));
	/*Item* corn = (Item*) Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);*/
	/*corn->position = Vector2D(385, 150);*/
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//L06 TODO 3: Call the function to load the map. 
	
	/*Engine::GetInstance().map->Load("Assets/Maps/", "Mapa.tmx");*/
	//instead of passing both path and name strings directly, you pass the variable in config storing these values
	Engine::GetInstance().map->Load(configParameters.child("map").attribute("path").as_string(), configParameters.child("map").attribute("name").as_string());

	//Load Parallax -> TODO: Pass parallax to config
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax").child("mountain1").attribute("path").as_string(), ParalaxType::Mountain1);/*(configParameters.child("map").child("parallax").child("mountain1").attribute("path").as_string()))*/
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax").child("mountain2").attribute("path").as_string(), ParalaxType::Mountain2);
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax").child("cloud1").attribute("path").as_string(), ParalaxType::Cloud1);
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax").child("moon").attribute("path").as_string(), ParalaxType::Moon);
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax").child("cloud2").attribute("path").as_string(), ParalaxType::Cloud2);
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax").child("cloud3").attribute("path").as_string(), ParalaxType::Cloud3);
	Engine::GetInstance().map->LoadParalax(configParameters.child("map").child("parallax").child("sky").attribute("path").as_string(), ParalaxType::Sky);
	
	
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

	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	SDL_DestroyTexture(img);

	return true;
}
<<<<<<< Updated upstream
=======

// Return the player position
Vector2D Scene::GetPlayerPosition()
{
	return player->GetPosition();
}

//Vector2D Scene::GetPhysPlayerPosition()
//{
//	return  { (float)METERS_TO_PIXELS(player->pbody->body->GetPosition().x), (float)METERS_TO_PIXELS(player->pbody->body->GetPosition().y) };
//}

void Scene::SaveGame()
{

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (result == NULL)
	{
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node sceneNode = loadFile.child("config").child("scene");

	//Save info to XML 

	//Player position
	sceneNode.child("entities").child("player").child("savedData").attribute("saved").set_value(true);
	sceneNode.child("entities").child("player").child("savedData").attribute("x").set_value(player->position.getY());
	sceneNode.child("entities").child("player").child("savedData").attribute("y").set_value(player->position.getY());
	sceneNode.child("entities").child("player").child("savedData").attribute("lives").set_value(player->lives);

	//enemies
	// ...

	//Saves the modifications to the XML 
	loadFile.save_file("config.xml");
}
>>>>>>> Stashed changes
