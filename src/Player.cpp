#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	//L03: TODO 2: Initialize Player parameters
	position = Vector2D(16, 16);
	return true;
}

bool Player::Start() {

	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load("Assets/Textures/GhostCharacter/Ghost_Sheet.png");

	dir = LEFT;

	

	//idle anim
	idle.PushBack({ 0, 0, 48, 48 });
	idle.PushBack({ 48, 0, 48, 48 });
	idle.PushBack({ 96, 0, 48, 48 });
	idle.PushBack({ 144, 0, 48, 48 });
	idle.PushBack({ 192, 0, 48, 48 });
	idle.PushBack({ 240, 0, 48, 48 });
	idle.PushBack({ 288, 0, 48, 48 });
	idle.PushBack({ 336, 0, 48, 48 });
	idle.speed = 0.2f;
	idle.loop = true;

	*currentAnim = idle;
	currentFrame = currentAnim->GetCurrentFrame();


	// L08 TODO 5: Add physics to the player - initialize physics body
	/*Engine::GetInstance().textures.get()->GetSize(texture, currentFrame.w, currentFrame.h);*/
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), GHOST_W, bodyType::DYNAMIC);

	// L08 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	
	

	//walkRight Anim
	/*walkRight.PushBack({ 0,48,48,48 });
	walkRight.PushBack({ 0,48, });*/

	


	return true;
}

bool Player::Update(float dt)
{
	currentFrame = currentAnim->GetCurrentFrame();
	// L08 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

	

	// Move left
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -0.2 * dt;
		isWalking = true;
		dir = RIGHT;
	}

	// Move right
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 0.2 * dt;
		isWalking = true;
		dir = LEFT;
		
	}
	
	//Jump
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping == false) {
		// Apply an initial upward force
		pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
		isJumping = true;
	}

	// If the player is jumpling, we don't want to apply gravity, we use the current velocity prduced by the jump
	if (isJumping == true)
	{
		velocity = { velocity.x, pbody->body->GetLinearVelocity().y };
	}

	// Apply the velocity to the player
	pbody->body->SetLinearVelocity(velocity);



	if (!isWalking && !isJumping) {
		currentAnim = &idle;
	}
	

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - currentFrame.w / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - currentFrame.h / 2);
	
	if (dir == LEFT) {
		Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentFrame);
	}
	else if (dir == RIGHT) {
		Engine::GetInstance().render.get()->DrawTextureFlipped(texture, (int)position.getX(), (int)position.getY(), &currentFrame);
	}
	
	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	Engine::GetInstance().textures.get()->UnLoad(texture);
	
	return true;
}

// L08 TODO 6: Define OnCollision function for the player. 
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		//reset the jump flag when touching the ground
		isJumping = false;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("End Collision PLATFORM");
		break;
	case ColliderType::ITEM:
		LOG("End Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("End Collision UNKNOWN");
		break;
	default:
		break;
	}
}