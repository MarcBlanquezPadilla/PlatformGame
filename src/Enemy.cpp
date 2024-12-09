#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
#include "Player.h"
#include "Audio.h"
//#include "EntityManager.h"

Enemy::Enemy() : Entity(EntityType::ENEMY)
{

}

Enemy::~Enemy() {
	delete pathfinding;
}

bool Enemy::Awake() {
	return true;
}

bool Enemy::Start() {

	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	currentAnimation = &idle;

	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	// Set the gravity of the body
	if (!parameters.attribute("gravity").as_bool()) pbody->body->SetGravityScale(0);

	dead = false;

	// Initialize pathfinding
	pathfinding = new Pathfinding();
	ResetPath();

	//SFX Node Load


	return true;
}

bool Enemy::Update(float dt)
{
	// Pathfinding testing inputs
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		Vector2D pos = GetPosition();
		Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
		pathfinding->ResetPath(tilePos);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		pathfinding->PropagateBFS();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_J) == KEY_REPEAT &&
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		pathfinding->PropagateBFS();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		pathfinding->PropagateDijkstra();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_K) == KEY_REPEAT &&
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		pathfinding->PropagateDijkstra();
	}

	// L13: TODO 3:	Add the key inputs to propagate the A* algorithm with different heuristics (Manhattan, Euclidean, Squared)

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_B) == KEY_DOWN) {
		pathfinding->PropagateAStar(MANHATTAN, position);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_B) == KEY_REPEAT &&
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		pathfinding->PropagateAStar(MANHATTAN, position);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_N) == KEY_DOWN) {
		pathfinding->PropagateAStar(EUCLIDEAN, position);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_N) == KEY_REPEAT &&
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		pathfinding->PropagateAStar(EUCLIDEAN, position);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {
		pathfinding->PropagateAStar(SQUARED, position);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_M) == KEY_REPEAT &&
		Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		pathfinding->PropagateAStar(SQUARED, position);
	}

	//if (player->hitEnemy) {

	//	player->hitEnemy = false;
	//	hitByPlayer = true;
	//}

	
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);



	

	return true;
}

bool Enemy::CleanUp()
{
	return true;
}

void Enemy::SetPosition(Vector2D pos) {
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Enemy::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}

void Enemy::ResetPath() {
	Vector2D pos = GetPosition();
	Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
	pathfinding->ResetPath(tilePos);
}

void Enemy::AddAnimation(Animation& anim, int startPosY, int frameSize, int numFrames)
{
	for (int i = 0; i < numFrames; i++)
	{
		anim.PushBack({ i*frameSize,startPosY,frameSize,frameSize });
	}
}

bool Enemy::CheckIfTwoPointsNear(Vector2D point1, Vector2D point2, float nearDistance)
{
	b2Vec2 vectorBetweenPoints = { point1.getX() - point2.getX(), point1.getY() - point2.getY() };
	return vectorBetweenPoints.Length()<nearDistance;
}

void Enemy::SetPlayer(Player* _player)
{
	player = _player;
}

void Enemy::SetPath(pugi::xml_node pathNode)
{
	route.clear();

	if (pathNode)
	{
		for (pugi::xml_node pointNode : pathNode.children("point")) {

			float x = pointNode.attribute("x").as_float();
			float y = pointNode.attribute("y").as_float();

			route.emplace_back( x, y );
		}

		for (int i = 0; i < route.size(); i++)
		{
			route[i] = Engine::GetInstance().map.get()->WorldToWorldCenteredOnTile(route[i].getX(), route[i].getY());
		}
		routeDestinationIndex = 0;
		destinationPoint = route[routeDestinationIndex];
	}
}


void Enemy::SaveData(pugi::xml_node enemyNode)
{
	enemyNode.attribute("dead").set_value(dead);
	enemyNode.attribute("x").set_value(pbody->GetPhysBodyWorldPosition().getX());
	enemyNode.attribute("y").set_value(pbody->GetPhysBodyWorldPosition().getY());
}


void Enemy::LoadData(pugi::xml_node enemyNode)
{
	pbody->SetPhysPositionWithWorld( enemyNode.attribute("x").as_float(), enemyNode.attribute("y").as_float() );
	ResetPath();
	dead = enemyNode.attribute("dead").as_bool();
	if (dead) {
		state = DEAD;
		pbody->body->SetEnabled(false);
	}
	
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	
	switch (physB->ctype)
	{
	case ColliderType::WEAPON:
		LOG("Enemy was hit by WEAPON");
		if(state != DEAD)
			DMGEnemy();
		break;
	case ColliderType::SHOT:
		LOG("Enemy was hit by SHOT");

		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::SPYKE:
		LOG("Collision SPYKE");
		break;

	case ColliderType::ENEMY:
		LOG("Collision ENEMY");
		break;
	case ColliderType::ABYSS:
	{
		LOG("Collision ABYSS");
		break;
	}
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;

	default:
		break;
	}
}

void Enemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("End Collision PLATFORM");
		break;
	case ColliderType::ITEM:

		LOG("End Collision ITEM");
		break;
	case ColliderType::LADDER:
		LOG("End Collision LADDER");
		break;
	case ColliderType::UNKNOWN:
		LOG("End Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Enemy::DMGEnemy() {
	deathTimer.Start();
	death.Reset();
	state = DEAD;
	/*dead = true;*/
}