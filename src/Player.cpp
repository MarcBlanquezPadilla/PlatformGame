#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Pathfinding.h"
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
	initPos.x = 25;
	initPos.y = 350;
	position = Vector2D(initPos.x, initPos.y);

	return true;
}

bool Player::Start() {

	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	t_texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("t_texture").as_string());
	t_texW = parameters.attribute("t_w").as_int();
	t_texH = parameters.attribute("t_h").as_int();
	

	idle.LoadAnimations(parameters.child("animations").child("idle"));
	walk.LoadAnimations(parameters.child("animations").child("walk"));
	jump.LoadAnimations(parameters.child("animations").child("jump"));
	fall.LoadAnimations(parameters.child("animations").child("fall"));
	hurt.LoadAnimations(parameters.child("animations").child("hurt"));
	death.LoadAnimations(parameters.child("animations").child("death"));

	t_idle.LoadAnimations(parameters.child("animations").child("t_idle"));
	t_walk.LoadAnimations(parameters.child("animations").child("t_walk"));
	t_jump.LoadAnimations(parameters.child("animations").child("t_jump"));
	t_fall.LoadAnimations(parameters.child("animations").child("t_fall"));
	t_spell1.LoadAnimations(parameters.child("animations").child("t_spell1"));
	t_spell2.LoadAnimations(parameters.child("animations").child("t_spell2"));
	t_hurt.LoadAnimations(parameters.child("animations").child("t_hurt"));
	t_death.LoadAnimations(parameters.child("animations").child("t_death"));

	jumpForce = parameters.child("propierties").attribute("jumpForce").as_float();
	pushForce = parameters.child("propierties").attribute("pushForce").as_float();
	moveSpeed = parameters.child("propierties").attribute("moveSpeed").as_float();
	friction = parameters.child("propierties").attribute("friction").as_float();
	gravity = parameters.child("propierties").attribute("gravity").as_float();
	hurtTime = parameters.child("propierties").attribute("hurtTime").as_float();
	attack1Time = parameters.child("propierties").attribute("attack1Time").as_float();
	attack2Time = parameters.child("propierties").attribute("attack2Time").as_float();
	respawnTime = parameters.child("propierties").attribute("respawnTime").as_float();
	playerState = (state)parameters.child("propierties").attribute("playerState").as_int();
	dir = (Direction)parameters.child("propierties").attribute("direction").as_int();
	lives = parameters.attribute("lives").as_int();

	

	if (parameters.child("savedData").attribute("saved").as_bool() == true)
	{
		position = { parameters.child("savedData").attribute("x").as_float(), parameters.child("savedData").attribute("y").as_float() };
		lives = parameters.child("savedData").attribute("lives").as_int();

	}
	
	destroyed = false;
	godMode = false;
	tpToStart = false;
	canClimb = false;
	transformed = false;
	reachedCheckPoint = false;

	saveGame = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/SaveGame.ogg");
	loadGame = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/LoadGame.ogg");
	gJumpSFX = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/ghostJump.ogg");
	pJumpSFX = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/pumpkinJump.ogg");
	gHurtSFX = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/SquidHit.ogg");
	pHurtSFX = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/BonkSFX.ogg");
	gDeathSFX = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/SinkOMO.ogg");
	pDeathSFX = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/PumpkinCrack.ogg");
	atk1SFX = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/atk1_alt.ogg");
	atk2SFX = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/customATK2.ogg");
	switchOnSFX = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/SwitchON.ogg");
	switchOffSFX = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/SwitchOFF.ogg");

	

	currentAnim = &idle;

	// L08 TODO 5: Add physics to the player - initialize physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), GHOST_W, bodyType::DYNAMIC);


	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetLinearDamping(friction);
	pbody->body->SetGravityScale(gravity);

	ATKcolliderW = 41;
	ATKcolliderH = 32;
	attackCollider = Engine::GetInstance().physics.get()->CreateRectangleSensor((int)position.getX(), (int)position.getY(), ATKcolliderW, ATKcolliderH, bodyType::DYNAMIC);
	attackCollider->ctype = ColliderType::WEAPON;
	attackCollider->body->SetEnabled(false);
	weaponOffset = { 23, 12 };
	pbody->listener = this;


	
	hurtTimer = Timer();
	respawnTimer = Timer();

	return true;
}

bool Player::Update(float dt)
{
	pbody->body->SetAwake(true);
	attackCollider->body->SetAwake(true);
	currentFrame = currentAnim->GetCurrentFrame();

	

	if (tpToStart)
	{
		pugi::xml_node playerNode = Engine::GetInstance().scene.get()->configParameters;
		Vector2D checkPointPos;
		checkPointPos.setX(playerNode.child("savedData").child("player").attribute("x").as_int());
		checkPointPos.setY(playerNode.child("savedData").child("player").attribute("y").as_int());

		b2Vec2 initPosInMeters = { PIXEL_TO_METERS(initPos.x), PIXEL_TO_METERS(initPos.y) };
		b2Vec2 checkPointInMeters = { PIXEL_TO_METERS(checkPointPos.getX()), PIXEL_TO_METERS(checkPointPos.getY()) };


		if (reachedCheckPoint && playerNode.child("savedData").attribute("saved").as_bool()) pbody->body->SetTransform(checkPointInMeters, 0.0f);
		else pbody->body->SetTransform(initPosInMeters, 0.0f);
		
		
		tpToStart = false;
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		godMode = !godMode;
		pbody->body->SetGravityScale(godMode == true || canClimb == true || playerState == DEAD ? 0 : gravity);
		pbody->body->SetLinearVelocity(godMode == true ? b2Vec2_zero : pbody->body->GetLinearVelocity());
		LOG("God mode = %d", (int)godMode);
	}


	velocity = b2Vec2_zero;

	if (transformable) {
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
			if (!transformed) {
				transformed = true;
				float pJumpForce = 0.5f;
				parameters.child("propierties").attribute("jumpForce").set_value(pJumpForce);
				Engine::GetInstance().audio.get()->PlayFx(switchOnSFX);
			}
			else {
				transformed = false;
				float gJumpForce = 0.6f;
				parameters.child("propierties").attribute("jumpForce").set_value(gJumpForce);
				
				Engine::GetInstance().audio.get()->PlayFx(switchOffSFX);
			}
			jumpForce = parameters.child("propierties").attribute("jumpForce").as_float();
			LOG("jump force = %f", parameters.child("propierties").attribute("jumpForce"));
		}
	}

	if (playerState !=HURT && playerState != DEAD && playerState != ATTACK1 && playerState != ATTACK2)
	{
		playerState = IDLE;
		

		// Move left
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			velocity.x = -moveSpeed * 16;
			playerState = WALK;
			dir = LEFT;
		}

		// Move right
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			velocity.x = moveSpeed * 16;
			playerState = WALK;
			dir = RIGHT;
		}

		if (godMode || canClimb)
		{
			velocity.y = 0;
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				velocity.y = -moveSpeed * 16;
				playerState = WALK;
			}

			// Move right
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				velocity.y = moveSpeed * 16;
				playerState = WALK;
			}
		}
		else
		{
			
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && VALUE_NEAR_TO_0(pbody->body->GetLinearVelocity().y)) {
				// Apply an initial upward force
				pbody->body->ApplyLinearImpulseToCenter(b2Vec2(0, -jumpForce), true);
				if(transformed) Engine::GetInstance().audio.get()->PlayFx(pJumpSFX);
				else Engine::GetInstance().audio.get()->PlayFx(gJumpSFX);
			}
			
			velocity = { velocity.x, pbody->body->GetLinearVelocity().y };

			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_X) == KEY_DOWN && transformed && playerState != ATTACK1) {
				
				playerState = ATTACK1;
				Engine::GetInstance().audio.get()->PlayFx(atk1SFX);
				attack1Timer.Start();
				pbody->body->SetLinearVelocity({ 0,0 });
				attackCollider->body->SetEnabled(true);
			
				
			}


			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_C) == KEY_DOWN && transformed && playerState != ATTACK2) {

				playerState = ATTACK2;
				attack2Timer.Start();
				pbody->body->SetLinearVelocity({ 0,0 });
				Engine::GetInstance().audio.get()->PlayFx(atk2SFX);
			}
		}
		
		

		pbody->body->SetLinearVelocity(velocity);
		/*attackCollider->body->SetLinearVelocity(velocity);*/

		if (pbody->body->GetLinearVelocity().y < -0.0001)
		{
			playerState = JUMP;
		}

		if (pbody->body->GetLinearVelocity().y > 0.0001)
		{
			playerState = FALL;
		}

		

	}
	else if (playerState == ATTACK1) {
		
		/*LOG("%f", attack1Timer.ReadSec());*/
		if (attack1Timer.ReadSec() >= attack1Time/* && t_spell1.HasFinished()*/) {
			playerState = IDLE;
			t_spell1.Reset();
			attackCollider->body->SetEnabled(false);
		}
		
		
	}
	else if (playerState == ATTACK2) {
		if (attack2Timer.ReadSec() >= attack2Time/* && t_spell2.HasFinished()*/) {
			playerState = IDLE;
			t_spell2.Reset();
		}
	}
	else if (playerState == HURT) {
		
		if (hurtTimer.ReadSec() >= hurtTime) {
			playerState = IDLE;
			hurt.Reset();
			t_hurt.Reset();
		}
		else
		{
			velocity = pbody->body->GetLinearVelocity();
			pbody->body->SetLinearVelocity(velocity);
		}
	}
	else if (playerState == DEAD) {

		

		pbody->body->SetLinearVelocity(b2Vec2_zero);
		if (respawnTimer.ReadSec() >= respawnTime) 
		{
			tpToStart = true;
			playerState = IDLE;
			dir = RIGHT;
			
			

			pbody->body->SetGravityScale(godMode == true || canClimb == true || playerState == DEAD ? 0 : gravity);
			
			if (lives <= 0) {
				lives = parameters.attribute("lives").as_int();
			}

			death.Reset();
			t_death.Reset();
		}
	}

	
	if (transformed) {
		switch (playerState) {
		case IDLE:
			
			currentAnim = &t_idle;
			break;
		case WALK:
			
			currentAnim = &t_walk;
			break;
		case JUMP:
			
			currentAnim = &t_jump;
			break;
		case FALL:
			
			currentAnim = &t_fall;
			break;
		case ATTACK1:
			currentAnim = &t_spell1;
			break;

		case ATTACK2:
			currentAnim = &t_spell2;
			break;

		case HURT:
			hurt.Reset();
			currentAnim = &t_hurt;
			break;
		case DEAD:
			
			currentAnim = &t_death;
			break;
		}
	}
	else {
		switch (playerState) {
		case IDLE:
			currentAnim = &idle;
			break;
		case WALK:
			currentAnim = &walk;
			break;
		case JUMP:
			currentAnim = &jump;
			break;
		case FALL:
			currentAnim = &fall;
			break;
		case HURT:
			currentAnim = &hurt;
			break;
		case DEAD:
			currentAnim = &death;
			break;
		}
	}


	
	b2Transform pbodyPos = pbody->body->GetTransform();
	//flip player texture according to direction
	if (!transformed) {

		
		position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2);
		position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

		if (dir == RIGHT) {
			Engine::GetInstance().render.get()->DrawTexture(texture, position.getX(), position.getY(), &currentFrame);
			
		}
		else if (dir == LEFT) {
			Engine::GetInstance().render.get()->DrawTextureFlipped(texture, position.getX(), position.getY(), &currentFrame);
		}
	}
	else {

		
		position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - t_texW / 2);
		position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - t_texH / 2);

		
	
		if (dir == RIGHT) {

			Engine::GetInstance().render.get()->DrawTexture(t_texture, position.getX(), position.getY(), &currentFrame);
			attackCollider->body->SetTransform({ attackCollider->body->GetPosition().x + weaponOffset.getX(),attackCollider->body->GetPosition().y }, pbody->GetRotation());
		}
		else if (dir == LEFT) {
			Engine::GetInstance().render.get()->DrawTextureFlipped(t_texture, position.getX(), position.getY(), &currentFrame);
			attackCollider->body->SetTransform({ attackCollider->body->GetPosition().x - weaponOffset.getX(),attackCollider->body->GetPosition().y }, pbody->GetRotation());
		}
		

		SDL_Rect attackRect = {
		(attackCollider->body->GetPosition().x) - ATKcolliderW / 2,
		(attackCollider->body->GetPosition().y) - ATKcolliderH / 2,
		ATKcolliderW,
		ATKcolliderH
		};
		

		/*Engine::GetInstance().render.get()->DrawRectangle(attackRect, 255, 255, 255, 255, false, true);*/
		
		
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
		transformable = true;
		break;
	case ColliderType::SPYKE:

		DMGPlayer(physA, physB);
			
		LOG("Collision SPYKE");
			
		break;

	case ColliderType::ENEMY:

		if (physA->ctype == ColliderType::WEAPON) {
			hitEnemy = true;
		}
		else {
			DMGPlayer(physA, physB);
		}
		

		LOG("Collision ENEMY");
		break;
	case ColliderType::ABYSS:
	{ 
		if (!godMode) {
			lives = parameters.attribute("lives").as_int();
			tpToStart = true;
		}
		LOG("Collision ABYSS");
		break;
	}
	case ColliderType::LADDER:
		canClimb = true;
		pbody->body->SetGravityScale(0);
		LOG("Collision LADDER");
		break;
	case ColliderType::CHECKPOINT:
		reachedCheckPoint = true;
		Engine::GetInstance().scene.get()->SaveState();
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
		transformable = false;
		LOG("End Collision ITEM");
		break;
	case ColliderType::LADDER:
		LOG("End Collision LADDER");
		canClimb = false;
		pbody->body->SetGravityScale(godMode == true || canClimb == true || playerState == DEAD ? 0 : gravity);
		break;
	case ColliderType::UNKNOWN:
		LOG("End Collision UNKNOWN");
		break;
	default:
		break;
	}
}


void Player::SetPosition(Vector2D pos) {
	if (!transformed) {
		b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
		pbody->body->SetTransform(bodyPos, 0);
	}
	else {
		b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
		pbody->body->SetTransform(bodyPos, 0);
	}
	
}

void Player::SaveData(pugi::xml_node playerNode)
{
	playerNode.attribute("x").set_value(pbody->GetPhysBodyWorldPosition().getX());
	playerNode.attribute("y").set_value(pbody->GetPhysBodyWorldPosition().getY());
	playerNode.attribute("lives").set_value(lives);
	playerNode.attribute("transformed").set_value(transformed);
}


void Player::LoadData(pugi::xml_node playerNode)
{
	position.setX(playerNode.attribute("x").as_int());
	position.setY(playerNode.attribute("y").as_int());
	lives = playerNode.attribute("lives").as_int();
	transformed = playerNode.attribute("transformed").as_bool();
	SetPosition(position);
}

void Player::DMGPlayer(PhysBody* physA, PhysBody* physB) {


	if (playerState != DEAD) {

		if (!godMode && playerState != HURT)
		{
			lives--;
			if (lives <= 0) {
				playerState = DEAD;

				if (transformed) Engine::GetInstance().audio.get()->PlayFx(pDeathSFX);
				else Engine::GetInstance().audio.get()->PlayFx(gDeathSFX);

				pbody->body->SetGravityScale(0);
				respawnTimer.Start();
			}
			else
			{

				hurtTimer.Start();
				playerState = HURT;
				pbody->body->SetGravityScale(godMode == true || canClimb == true || playerState == DEAD ? 0 : gravity);

				if (transformed) Engine::GetInstance().audio.get()->PlayFx(pHurtSFX);
				else Engine::GetInstance().audio.get()->PlayFx(gHurtSFX);
				
				pushDir = b2Vec2_zero;
				pushDir.x = physA->body->GetPosition().x - physB->body->GetPosition().x;
				pushDir.y = physA->body->GetPosition().y - physB->body->GetPosition().y;
				pushDir.Normalize();
				
				physA->body->SetLinearVelocity(b2Vec2_zero);
				physA->body->ApplyLinearImpulseToCenter(b2Vec2(pushForce * pushDir.x, pushForce * pushDir.y), true);
				

			}
		}
	}
	
	
}