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

	walk.PushBack({ 0, 48, 48, 48 });
	walk.PushBack({ 48, 48, 48, 48 });
	walk.PushBack({ 96, 48, 48, 48 });
	walk.PushBack({ 144, 48, 48, 48 });
	walk.speed = 0.1f;
	walk.loop = true;

	jump.PushBack({ 0, 96, 48, 48 });
	jump.PushBack({ 48, 96, 48, 48 });
	jump.PushBack({ 96, 96, 48, 48 });
	jump.speed = 0.1f;
	jump.loop = false;

	fall.PushBack({ 0, 144, 48, 48 });
	fall.PushBack({ 48, 144, 48, 48 });
	fall.PushBack({ 96, 144, 48, 48 });
	fall.speed = 0.1f;
	fall.loop = false;

	hurt.PushBack({ 0, 192, 48, 48 });
	hurt.PushBack({ 48, 192, 48, 48 });
	hurt.PushBack({ 96, 192, 48, 48 });
	hurt.PushBack({ 144,192, 48, 48 });
	hurt.speed = 0.2f;
	hurt.loop = false;

	death.PushBack({ 0, 240, 48, 48 });
	death.PushBack({ 48, 240, 48, 48 });
	death.PushBack({ 96, 240, 48, 48 });
	death.PushBack({ 144, 240, 48, 48 });
	death.PushBack({ 192, 240, 48, 48 });
	death.PushBack({ 240, 240, 48, 48 });
	death.speed = 0.1f;
	death.loop = false;
	


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

	destroyed = false;
	currentAnim = &idle;

	// L08 TODO 5: Add physics to the player - initialize physics body
	/*Engine::GetInstance().textures.get()->GetSize(texture, currentFrame.w, currentFrame.h);*/
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), GHOST_W, bodyType::DYNAMIC);

	// L08 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	return true;
}

bool Player::Update(float dt)
{

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
		LOG("God mode = %d", (int)godMode);
	}
	
	// L08 TODO 5: Add physics to the player - updated player position using physics
	b2Vec2 velocity = b2Vec2(0, -GRAVITY_Y);

	// Move left
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		velocity.x = -0.2 * dt;
		isWalking = true;
		dir = LEFT;
	}

	// Move right
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		velocity.x = 0.2 * dt;
		isWalking = true;
		dir = RIGHT;
		
	}
<<<<<<< HEAD

	if (godMode)
	{
		velocity.y = 0;
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
			velocity.y = -0.2 * dt;
			isWalking = true;
		}

		// Move right
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
			velocity.y = 0.2 * dt;
			isWalking = true;

		}
	}
	else
	{
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping == false) {
			// Apply an initial upward force
			pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
			isJumping = true;
		}

		// If the player is jumpling, we don't want to apply gravity, we use the current velocity prduced by the jump
		if (isJumping == true)
		{
			velocity = { velocity.x, pbody->body->GetLinearVelocity().y};
		}
=======

	if (isWalking && currentAnim != &walk) {
		walk.Reset();
		currentAnim = &walk;

	}
	
	//Jump
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isJumping == false) {
		// Apply an initial upward force
		pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
		isJumping = true;
	}

	// If the player is jumpling, we don't want to apply gravity, we use the current velocity prduced by the jump
	if (isJumping == true && currentAnim != &jump)
	{
		velocity = { velocity.x, pbody->body->GetLinearVelocity().y };
		if (velocity.y > 0) {
			fall.Reset();
			currentAnim = &fall;
		}
		else 
		{
			jump.Reset();
			currentAnim = &jump;
		}
		

>>>>>>> 736163413861874d09dbb9d53152ba75702723b3
	}

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_A == KEY_UP) || Engine::GetInstance().input->GetKey(SDL_SCANCODE_D == KEY_UP)) {
		if (isWalking && !isJumping) {
			isWalking = false;
			currentAnim = &idle;
		}
		
	}

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_S == KEY_IDLE)
		&& Engine::GetInstance().input->GetKey(SDL_SCANCODE_W == KEY_IDLE)
		&& Engine::GetInstance().input->GetKey(SDL_SCANCODE_A == KEY_IDLE)
		&& Engine::GetInstance().input->GetKey(SDL_SCANCODE_D == KEY_IDLE))
	{
		idle.Reset();
		currentAnim = &idle;
	}

	// Apply the velocity to the player
	pbody->body->SetLinearVelocity(velocity);
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - currentFrame.w / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - currentFrame.h / 2);
	
	if (dir == LEFT) {
		Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentFrame);
	}
	else if (dir == RIGHT) {
		Engine::GetInstance().render.get()->DrawTextureFlipped(texture, (int)position.getX(), (int)position.getY(), &currentFrame);
	}
	

	currentAnim->Update();

	if (!destroyed)
	{
		SDL_Rect rect = currentAnim->GetCurrentFrame();
		if (dir == RIGHT) {
			Engine::GetInstance().render.get()->DrawTexture(texture, position.getX() - rect.w / 2, position.getY() - rect.h / 2, &rect);
		}
		else if (dir == LEFT) {
			Engine::GetInstance().render.get()->DrawTextureFlipped(texture, position.getX() - rect.w / 2, position.getY() - rect.h / 2, &rect);
		}
	}


	return true;
}

bool Player::PostUpdate(float dt) {

	
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
	case ColliderType::SPYKE:
		LOG("Collision SPYKE");
		if (currentAnim != &hurt) {
			hurt.Reset();
			currentAnim = &hurt;
		}
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