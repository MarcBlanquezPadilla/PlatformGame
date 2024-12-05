#include "GroundEnemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
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
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_float());
	position.setY(parameters.attribute("y").as_float());
	texW = parameters.attribute("w").as_float();
	texH = parameters.attribute("h").as_float();
	drawOffsetX = 0;
	drawOffsetY = 0;

	//INIT ANIMS
	AddAnimation(idle, 0, texW, 1);
	idle.speed = 0.2f;
	

	AddAnimation(walk, 0, texW, 4);
	idle.speed = 0.2f;
	

	AddAnimation(attack, 48, texW, 4);
	AddAnimation(attack, 0, texW, 1);
	attack.speed = 0.2f;
	attack.loop = false;


	currentAnimation = &idle;
	canAttack = false;
	attacked = false;

	//INIT ROUTE
	for (int i = 0; i < route.size(); i++)
	{
		route[i] = Engine::GetInstance().map.get()->WorldToWorldCenteredOnTile(route[i].getX(), route[i].getY());
	}
	routeDestinationIndex = 0;
	destinationPoint = route[routeDestinationIndex];

	//INIT PHYSICS
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(),  32/4, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;
	pbody->body->SetGravityScale(1.2f);
	pbody->body->SetFixedRotation(true);
	pbody->body->SetTransform({ PIXEL_TO_METERS(destinationPoint.getX()), PIXEL_TO_METERS(destinationPoint.getY()) }, 0);

	//INIT PATH
	pathfinding = new Pathfinding();
	ResetPath();

	//INIT VARIABLES
	state = PATROL;
	speed = parameters.child("properties").attribute("speed").as_float();
	chaseArea = parameters.child("properties").attribute("chaseArea").as_float();
	attackArea = parameters.child("properties").attribute("attackArea").as_float();
	jumpForce = parameters.child("properties").attribute("jumpForce").as_float();
	attackTime = parameters.child("properties").attribute("attackTime").as_float();
	dir = LEFT;

	return true;
}

bool GroundEnemy::Update(float dt) {


	dist = pbody->GetPhysBodyWorldPosition().distanceEuclidean(player->pbody->GetPhysBodyWorldPosition());

	//STATES CHANGERS
	if (dist > chaseArea && state != PATROL)
	{
		state = PATROL;
		ResetPath();
		destinationPoint = route[routeDestinationIndex];
	}
	
	else if (dist <= chaseArea/* && state != CHASING*/)
	{
		

		if (pbody->GetPhysBodyWorldPosition().distanceEuclidean(player->pbody->GetPhysBodyWorldPosition()) <= attackArea && state != ATTACK) {
			state = ATTACK;
		}
		
		
		else if (state != CHASING) 
		{
			state = CHASING;
			ResetPath();
		}
		
	}
	

	Vector2D playerPos = player->pbody->GetPhysBodyWorldPosition();
	Vector2D playerPosCenteredOnTile = Engine::GetInstance().map.get()->WorldToWorldCenteredOnTile(playerPos.getX(), playerPos.getY());

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
	else if (state == CHASING) {


		
		if (destinationPoint != playerPosCenteredOnTile)
		{
			destinationPoint = playerPosCenteredOnTile;
			ResetPath();
		}

		
	}
	else if (state == ATTACK) {
		/*Vector2D castPos = player->pbody->GetPhysBodyWorldPosition();*/
		
		/*Vector2D endCastPos = castPos + (dir == LEFT ? - 1 : 1);*/
		
		/*pbody->RayCast(castPos.getX(), castPos.getY(), );*/

		currentAnimation = &attack;
	
		player->DMGPlayer(player->pbody, pbody);

		LOG("lives: %d", player->lives);
		LOG("dist: %d", dist);
		if (dist > attackArea) {
			state = PATROL;
			
		}
		if (attack.HasFinished()) {
			attack.Reset();
			
			player->playerState = IDLE;
			

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

	Vector2D currentTile = Engine::GetInstance().map.get()->WorldToMap(pbody->GetPhysBodyWorldPosition().getX(), pbody->GetPhysBodyWorldPosition().getY());

	if (pathfinding->IsJumpable(currentTile.getX(), currentTile.getY()) && VALUE_NEAR_TO_0(pbody->body->GetLinearVelocity().LengthSquared()))
	{
		pbody->body->ApplyLinearImpulseToCenter({ 0, -jumpForce }, true);
	}

	

	//DIRECTION
	if (pbody->body->GetLinearVelocity().x > 0.2f) {
		dir = RIGHT;
	}
	else if (pbody->body->GetLinearVelocity().x < -0.2f){
		dir = LEFT;
	}

	//DRAW
	if (Engine::GetInstance().GetDebug())
	{
		Engine::GetInstance().render.get()->DrawCircle(position.getX() + texW / 2, position.getY() + texH / 2, chaseArea * 2, 255, 255, 255);
		Engine::GetInstance().render.get()->DrawCircle(destinationPoint.getX(), destinationPoint.getY(), 3, 255, 0, 0);
		pathfinding->DrawPath();
	}

	currentAnimation->Update();

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2 + drawOffsetX);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 1.5 + drawOffsetY);

	if (dir == LEFT) {
		Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	}
	else if (dir == RIGHT) {
		Engine::GetInstance().render.get()->DrawTextureFlipped(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	}

	return true;
}





