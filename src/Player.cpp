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
	initPos = b2Vec2_zero;
	initPos.x = parameters.attribute("x").as_float();
	initPos.y = parameters.attribute("y").as_float();
	position = Vector2D(initPos.x, initPos.y);

	return true;
}

bool Player::Start() {

	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	/*get()->Load("Assets/Textures/GhostCharacter/Ghost_Sheet.png");*/

	destroyed = false;

	idle.LoadAnimations(parameters.child("animations").child("idle"));
	walk.LoadAnimations(parameters.child("animations").child("walk"));
	jump.LoadAnimations(parameters.child("animations").child("jump"));
	fall.LoadAnimations(parameters.child("animations").child("fall"));
	hurt.LoadAnimations(parameters.child("animations").child("hurt"));
	death.LoadAnimations(parameters.child("animations").child("death"));
	
	currentAnim = &idle;

	// L08 TODO 5: Add physics to the player - initialize physics body
	/*Engine::GetInstance().textures.get()->GetSize(texture, currentFrame.w, currentFrame.h);*/
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), GHOST_W, bodyType::DYNAMIC);

	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	lives = parameters.attribute("lives").as_int();
	hurtTimer = Timer();
	respawnTimer = Timer();

	return true;
}

bool Player::Update(float dt)
{
	currentFrame = currentAnim->GetCurrentFrame();

	if (tpToStart)
	{
		b2Vec2 initPosInMeters = { PIXEL_TO_METERS(initPos.x), PIXEL_TO_METERS(initPos.y) };
		pbody->body->SetTransform(initPosInMeters, 0.0f);
		tpToStart = false;
		if (lives <= 0) {
			lives = parameters.attribute("lives").as_int();
		}
		LOG("Lives: %i", lives);
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
		pbody->body->SetGravityScale(godMode == true ? 0 : 1);
		pbody->body->SetLinearVelocity(godMode == true ? b2Vec2_zero : pbody->body->GetLinearVelocity());
		LOG("God mode = %d", (int)godMode);
	}


	b2Vec2 velocity = b2Vec2_zero;

	if (playerState !=HURT && playerState != DEAD)
	{
		playerState = IDLE;

		// Move left
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			velocity.x = -0.2 * dt;
			playerState = WALK;
			dir = LEFT;
		}

		// Move right
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			velocity.x = 0.2 * dt;
			playerState = WALK;
			dir = RIGHT;
		}

		if (godMode)
		{
			velocity.y = 0;
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				velocity.y = -0.2 * dt;
				playerState = WALK;
			}

			// Move right
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				velocity.y = 0.2 * dt;
				playerState = WALK;
			}
		}
		else
		{
			
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && VALUE_NEAR_TO_0(pbody->body->GetLinearVelocity().y)) {
				// Apply an initial upward force
				pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
			}
			
			velocity = { velocity.x, pbody->body->GetLinearVelocity().y };
		}


		pbody->body->SetLinearVelocity(velocity);

		if (pbody->body->GetLinearVelocity().y < -0.0001)
		{
			playerState = JUMP;
		}

		if (pbody->body->GetLinearVelocity().y > 0.0001)
		{
			playerState = FALL;
		}

		

	} else if (playerState == HURT) {
		

		
		if (lives <= 0) {
			playerState = DEAD;
			respawnTimer.Start();
		}
		else {
			if (hurtTimer.ReadSec() >= hurtTime && hurt.HasFinished()) playerState = IDLE;
		}
	}
	else if (playerState == DEAD) {

		
		if (respawnTimer.ReadSec() >= respawnTime && death.HasFinished()) 
		{

			playerState = IDLE;
			lives = parameters.child("entity").child("player").attribute("lives").as_int();
			
			tpToStart = true;
		}

	
	}

	

	if (playerState != previousState) {
		switch (playerState) {
		case IDLE:
			idle.Reset();
			currentAnim = &idle;
			break;
		case WALK:
			walk.Reset();
			currentAnim = &walk;
			break;
		case JUMP:
			jump.Reset();  // Solo resetear si cambió el estado
			currentAnim = &jump;
			break;
		case FALL:
			fall.Reset();  // Solo resetear si cambió el estado
			currentAnim = &fall;
			break;
		case HURT:
			hurt.Reset();
			currentAnim = &hurt;
			break;
		case DEAD:
			death.Reset();
			currentAnim = &death;
			break;
		}
	}

	previousState = playerState;

	
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - currentFrame.w / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - currentFrame.h / 2);
	
	
	
	if (dir == RIGHT) {
		Engine::GetInstance().render.get()->DrawTexture(texture, position.getX(), position.getY(), &currentFrame);
	}
	else if (dir == LEFT) {
		Engine::GetInstance().render.get()->DrawTextureFlipped(texture, position.getX(), position.getY(), &currentFrame);
	}
	



	currentAnim->Update();

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
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::SPYKE:

		if (playerState != DEAD) {
			if (!godMode && playerState != HURT)
			{
				lives--;
				LOG("Lives: %i", lives);
				hurtTimer.Start();
				playerState = HURT;


			}
			pushDir = b2Vec2_zero;
			pushDir.x = physA->body->GetPosition().x - physB->body->GetPosition().x;
			pushDir.y = physA->body->GetPosition().y - physB->body->GetPosition().y;

			pushDir.Normalize();
			physA->body->ApplyLinearImpulseToCenter(b2Vec2(pushForce * pushDir.x, pushForce * pushDir.y), true);
			LOG("Collision SPYKE");
		}
		
		break;

	case ColliderType::ABYSS:
	{ 
		if (!godMode && playerState != HURT) {
			playerState = HURT;
			tpToStart = true;
		}
		
		
		
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