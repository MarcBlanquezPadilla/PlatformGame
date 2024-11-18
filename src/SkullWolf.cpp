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
	texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/Enemies/Skull Wolf/SkullWolf_Sprite Sheet.png");
	position.setX(250);
	position.setY(350);
	texW = 64;
	texH = 64;


	//Load animations
	/*idle.LoadAnimations(parameters.child("animations").child("idle"));*/
	idle.PushBack({0,0,64,64});
	idle.PushBack({ 64,0,64,64 });
	idle.PushBack({ 128,0,64,64 });
	idle.PushBack({ 192,0,64,64 });
	idle.PushBack({ 256,0,64,64 });
	idle.PushBack({ 320,0,64,64 });
	idle.speed = 0.2f;
	currentAnimation = &idle;

	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateRectangle((int)position.getX() + texW / 2, (int)position.getY() + texH / 2, texW, texH / 2, bodyType::DYNAMIC);

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
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2 - texH / 4);
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	
	return true;
}

