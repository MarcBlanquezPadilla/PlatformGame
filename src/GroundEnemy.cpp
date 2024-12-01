#include "GroundEnemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Physics.h"
#include "Scene.h"
#include "Player.h"
#include "LOG.h"

GroundEnemy::GroundEnemy()
{

}

GroundEnemy::~GroundEnemy() {
	delete pathfinding;
}

bool GroundEnemy::Start() {
	texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Enemies/Bat/Root/BatRoot_Sheet.png");
	position.setX(100);
	position.setY(420);
	texW = 32;
	texH = 32;
	drawOffsetX = 0;
	drawOffsetY = 0;

	//INIT ANIMS
	AddAnimation(idle, 0, 32, 4);
	idle.speed = 0.2f;
	currentAnimation = &idle;

	//INIT ROUTE
	for (int i = 0; i < route.size(); i++)
	{
		route[i] = Engine::GetInstance().map.get()->WorldToWorldCenteredOnTile(route[i].getX(), route[i].getY());
	}
	routeDestinationIndex = 0;
	destinationPoint = route[routeDestinationIndex];

	//INIT PHYSICS
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), 32 / 4, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;
	pbody->body->SetGravityScale(1.2f);
	pbody->body->SetFixedRotation(true);
	pbody->body->SetTransform({ PIXEL_TO_METERS(destinationPoint.getX()), PIXEL_TO_METERS(destinationPoint.getY()) }, 0);

	//INIT PATH
	pathfinding = new Pathfinding();
	ResetPath();

	//INIT VARIABLES
	speed = 1;
	jumpForce = 0.3f;
	state = PATROL;
	chaseArea = 100;


	return true;
}

bool GroundEnemy::Update(float dt) {


	//STATES CHANGERS
	if (pbody->GetPhysBodyWorldPosition().distanceEuclidean(player->pbody->GetPhysBodyWorldPosition()) > chaseArea && state != PATROL)
	{
		state = PATROL;
		ResetPath();
		destinationPoint = route[routeDestinationIndex];
	}
	else if (pbody->GetPhysBodyWorldPosition().distanceEuclidean(player->pbody->GetPhysBodyWorldPosition()) <= chaseArea && state != CHASING)
	{
		state = CHASING;
		ResetPath();
	}

	//STATES CONTROLER
	if (state == PATROL) {

		Vector2D physPos = pbody->GetPhysBodyWorldPosition();
		if (CheckIfTwoPointsNear(destinationPoint, { physPos.getX(), physPos.getY() }, 7))
		{
			routeDestinationIndex++;
			if (routeDestinationIndex == route.size()) routeDestinationIndex = 0;
			destinationPoint = route[routeDestinationIndex];
			ResetPath();
		}
	}
	else {
		Vector2D playerPos = player->pbody->GetPhysBodyWorldPosition();
		Vector2D playerPosCenteredOnTile = Engine::GetInstance().map.get()->WorldToWorldCenteredOnTile(playerPos.getX(), playerPos.getY());
		if (destinationPoint != playerPosCenteredOnTile)
		{
			destinationPoint = playerPosCenteredOnTile;
			ResetPath();
		}
	}

	//PATHFINDING CONTROLER
	if (pathfinding->pathTiles.empty())
	{
		while (pathfinding->pathTiles.empty())
		{
			pathfinding->PropagateAStar(SQUARED, destinationPoint, Pathfinding::WALK);

		}
		pathfinding->pathTiles.pop_back();
	}
	else
	{

		Vector2D nextTile = pathfinding->pathTiles.back();
		Vector2D nextTileWorld = Engine::GetInstance().map.get()->MapToWorldCentered(nextTile.getX(), nextTile.getY());

		if (CheckIfTwoPointsNear(nextTileWorld, { (float)METERS_TO_PIXELS(pbody->body->GetPosition().x), (float)METERS_TO_PIXELS(pbody->body->GetPosition().y) }, 3)) {

			//if (pathfinding->IsJumpable(pathfinding->pathTiles.back().getX(), pathfinding->pathTiles.back().getY())) pbody->body->ApplyLinearImpulseToCenter({ 0, -jumpForce }, true);
			pathfinding->pathTiles.pop_back();
			if (pathfinding->pathTiles.empty()) ResetPath();
		}
		else {
			Vector2D nextTilePhysics = { PIXEL_TO_METERS(nextTileWorld.getX()),PIXEL_TO_METERS(nextTileWorld.getY()) };
			b2Vec2 direction = { nextTilePhysics.getX() - pbody->body->GetPosition().x, nextTilePhysics.getY() - pbody->body->GetPosition().y };
			direction.Normalize();
			pbody->body->SetLinearVelocity({ direction.x * speed, pbody->body->GetLinearVelocity().y});
		}
	}

	//DRAW
	pathfinding->DrawPath();

	currentAnimation->Update();

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2 + drawOffsetX);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2 + drawOffsetY);
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	Engine::GetInstance().render.get()->DrawCircle(destinationPoint.getX(), destinationPoint.getY(), 3, 255, 0, 0);

	return true;
}





