#include "BatEnemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Physics.h"
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

	//Load animations
	/*idle.LoadAnimations(parameters.child("animations").child("idle"));*/
	AddAnimation(idle, 0, 32, 4);

	idle.speed = 0.2f;
	currentAnimation = &idle;

	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), 32 / 4, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	speed = 1;

	// Set the gravity of the body
	if (!parameters.attribute("gravity").as_bool()) pbody->body->SetGravityScale(1.0f);
	pbody->body->SetFixedRotation(true);

	// Initialize pathfinding
	pathfinding = new Pathfinding();
	ResetPath();

	return true;
}

bool BatEnemy::Update(float dt) {
	LOG("%d", pathfinding->pathTiles.size());

	if (pathfinding->pathTiles.empty())
		pathfinding->PropagateAStar(SQUARED, {200,200});
	else {

		Vector2D nextTile = pathfinding->pathTiles.back();
		Vector2D nextTileWorld = Engine::GetInstance().map.get()->MapToWorldCentered(nextTile.getX(), nextTile.getY());
		Vector2D nextTilePhysics = { PIXEL_TO_METERS(nextTileWorld.getX()),PIXEL_TO_METERS(nextTileWorld.getY()) };

		Engine::GetInstance().render.get()->DrawCircle(nextTileWorld.getX(), nextTileWorld.getY(), 2, 255, 0, 0, 255);

		// Calcular la dirección hacia el próximo tile
		b2Vec2 direction = { nextTilePhysics.getX()-pbody->body->GetPosition().x, nextTilePhysics.getY() - pbody->body->GetPosition().y };
		float distanceToTile = direction.Length();

		// Si estamos cerca del tile objetivo, eliminarlo de la lista
		if (distanceToTile < PIXEL_TO_METERS(3)) { // 5 píxeles de tolerancia

			pathfinding->pathTiles.pop_back();
			if (pathfinding->pathTiles.empty()) ResetPath();
		}
		else {
			// Normalizar la dirección y mover al enemigo hacia el tile
			direction.Normalize();
			pbody->body->SetLinearVelocity({direction.x * speed, direction.y * speed});
		}
	}



	//DRAW
	pathfinding->DrawPath();
	

	currentAnimation->Update();

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2 + drawOffsetX);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2 + drawOffsetY);
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());

	return true;
}



