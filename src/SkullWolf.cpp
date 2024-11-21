#include "SkullWolf.h"
#include "Engine.h"
#include "Textures.h"
#include "Physics.h"

SkullWolf::SkullWolf()
{

}

SkullWolf::~SkullWolf() {
	delete pathfinding;
}

bool SkullWolf::Start() {
	texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Enemies/Skeleton Warrior.png");
	position.setX(250);
	position.setY(350);
	texW = 48;
	texH = 48;
	drawOffsetX = 3;
	drawOffsetY = -3;


	//Load animations
	/*idle.LoadAnimations(parameters.child("animations").child("idle"));*/
	AddAnimation(idle, 0, 48, 8);

	idle.speed = 0.2f;
	currentAnimation = &idle;

	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX() + texW/2, (int)position.getY()+texH/3, 32 / 2, 32, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	

	// Set the gravity of the body
	if (!parameters.attribute("gravity").as_bool()) pbody->body->SetGravityScale(1.0f);
	pbody->body->SetFixedRotation(true);

	// Initialize pathfinding
	pathfinding = new Pathfinding();
	ResetPath();

	return true;
}

bool SkullWolf::Update(float dt) {
	
	currentAnimation->Update();

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2 + drawOffsetX);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2 + drawOffsetY);
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	
	//pathfinding->PropagateAStar(SQUARED, { 500,500 });

	return true;
}

