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

	Item* lolly = (Item*) Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	lolly->position = Vector2D();
	
	//L08 Create a new item using the entity manager and set the position to (200, 672) to test
	Item* corn = (Item*) Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM);
	corn->position = Vector2D(385, 150);
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//L06 TODO 3: Call the function to load the map. 
	
	Engine::GetInstance().map->Load("Assets/Maps/", "Mapa.tmx");
	Engine::GetInstance().map->LoadParalax("Assets/Textures/Parallax/Mountain1X2.png", ParalaxType::Mountain1);
	Engine::GetInstance().map->LoadParalax("Assets/Textures/Parallax/Mountain2X2.png", ParalaxType::Mountain2);
	Engine::GetInstance().map->LoadParalax("Assets/Textures/Parallax/Cloud1X2.png", ParalaxType::Cloud1);
	Engine::GetInstance().map->LoadParalax("Assets/Textures/Parallax/MoonX2.png", ParalaxType::Moon);
	Engine::GetInstance().map->LoadParalax("Assets/Textures/Parallax/Cloud2X2.png", ParalaxType::Cloud2);
	Engine::GetInstance().map->LoadParalax("Assets/Textures/Parallax/Cloud3X2.png", ParalaxType::Cloud3);
	Engine::GetInstance().map->LoadParalax("Assets/Textures/Parallax/SkyX2.png", ParalaxType::Sky);
	
	
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
