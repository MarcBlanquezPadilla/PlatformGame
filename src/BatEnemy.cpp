#include "BatEnemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Physics.h"
#include "Scene.h"
#include "Player.h"
#include "LOG.h"

BatEnemy::BatEnemy()
{

}

BatEnemy::~BatEnemy() {
	delete pathfinding;
}

bool BatEnemy::Start() {
	texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Enemies/Bat/Vampire/BatVampire_Sheet.png");
	position.setX(250);
	position.setY(350);
	texW = 32;
	texH = 32;
	drawOffsetX = 0;
	drawOffsetY = 0;


	AddAnimation(idle, 0, 32, 4);

	idle.speed = 0.2f;
	currentAnimation = &idle;

	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), 32 / 4, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	chaseArea = 100;

	speed = 1;
	state = PATROL;


	// Set the gravity of the body
	pbody->body->SetGravityScale(0);
	pbody->body->SetFixedRotation(true);

	// Initialize pathfinding
	pathfinding = new Pathfinding();
	ResetPath();
	
	// Relocate route points to center
	for (int i = 0; i < route.size(); i++)
	{
		route[i] = Engine::GetInstance().map.get()->WorldToWorldCenteredOnTile(route[i].getX(), route[i].getY());
	}
	routeDestinationIndex = 0;
	destinationPoint = route[routeDestinationIndex];

	return true;
}

bool BatEnemy::Update(float dt) {

	Vector2D playerPos = Engine::GetInstance().scene.get()->GetPhysPlayerPosition();

	//STATES CONTROLER
	if (state == PATROL) {

		if (CheckIfTwoPointsNear(destinationPoint, { (float)METERS_TO_PIXELS(pbody->body->GetPosition().x), (float)METERS_TO_PIXELS(pbody->body->GetPosition().y) }, 5))
		{
			routeDestinationIndex++;
			if (routeDestinationIndex == route.size()) routeDestinationIndex = 0;
			destinationPoint = route[routeDestinationIndex];
			ResetPath();
		}
	}
	else {
		
		destinationPoint = playerPos;
	}
	
	

	if (position.distanceEuclidean(playerPos) < chaseArea && state == PATROL) {
		state = CHASING;
	}
	if (position.distanceEuclidean(playerPos) > chaseArea && state == CHASING) {
		state = PATROL;
	}
	
	
	//PATHFINDING CONTROLLER
	if (pathfinding->pathTiles.empty()) 
	{
		pbody->body->SetLinearVelocity({ 0, 0 });
		pathfinding->PropagateAStar(SQUARED, destinationPoint);
	}	
	else 
	{

		Vector2D nextTile = pathfinding->pathTiles.back();
		Vector2D nextTileWorld = Engine::GetInstance().map.get()->MapToWorldCentered(nextTile.getX(), nextTile.getY());


		if (CheckIfTwoPointsNear(nextTileWorld, {(float)METERS_TO_PIXELS(pbody->body->GetPosition().x), (float)METERS_TO_PIXELS(pbody->body->GetPosition().y)}, 3)) {

			pathfinding->pathTiles.pop_back();
			if (pathfinding->pathTiles.empty()) ResetPath();
		}
		else {
			Vector2D nextTilePhysics = { PIXEL_TO_METERS(nextTileWorld.getX()),PIXEL_TO_METERS(nextTileWorld.getY()) };
			b2Vec2 direction = { nextTilePhysics.getX() - pbody->body->GetPosition().x, nextTilePhysics.getY() - pbody->body->GetPosition().y };
			direction.Normalize();
			pbody->body->SetLinearVelocity({direction.x * speed, direction.y * speed});
		}
	}

	//DRAW
	pathfinding->DrawPath();

	if (Engine::GetInstance().physics.get()->debug) {
		Engine::GetInstance().render.get()->DrawCircle(playerPos.getX(), playerPos.getY(), 3, 255, 0, 0);
	}
	
	currentAnimation->Update();

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2 + drawOffsetX);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2 + drawOffsetY);
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());

	return true;
}





