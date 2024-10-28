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

	

	return true;
}
