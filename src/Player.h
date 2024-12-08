#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "Timer.h"


#define GHOST_W 9

struct SDL_Texture;

enum state {
	IDLE = 0,
	WALK,
	JUMP,
	FALL,
	HURT,
	ATTACK1,
	ATTACK2,
	DEAD
};

enum Direction {

	LEFT,
	RIGHT
};

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate(float dt);

	bool CleanUp();

	// L08 TODO 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void SetPosition(Vector2D pos);

	void SaveData(pugi::xml_node playerNode);

	void LoadData(pugi::xml_node playerNode);

	void DMGPlayer(PhysBody* physA, PhysBody* physB);

public:

	//Declare player parameters
	PhysBody* attackCollider;
	//PhysBody* shotCollider;

	int ATKcolliderW;
	int ATKcolliderH;
	Vector2D weaponOffset;
	
	SDL_Texture* texture;
	SDL_Texture* t_texture;
	int texW, texH;
	int t_texW, t_texH;
	SDL_Rect currentFrame;

	// L08 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	float jumpForce; // The force to apply when jumping
	float pushForce; // The force to apply when getting pushed;
	float moveSpeed;
	float friction;
	float gravity;

	bool destroyed;
	bool godMode;
	bool tpToStart;
	bool canClimb;
	bool transformable;
	bool transformed;
	bool reachedCheckPoint;
	bool hitEnemy;
	bool shot;

	Timer attack1Timer;
	float attack1Time;
	Timer attack2Timer;
	float attack2Time;

	Timer hurtTimer;
	float hurtTime;
	
	Timer respawnTimer;
	float respawnTime;

	b2Vec2 velocity = b2Vec2_zero;
	b2Vec2 initPos;
	b2Vec2 pushDir;

	pugi::xml_node parameters;


	int lives;

	Animation* currentAnim = nullptr;

	Animation idle;
	Animation walk;
	Animation jump;
	Animation fall;
	Animation hurt;
	Animation death;

	Animation t_idle;
	Animation t_walk;
	Animation t_jump;
	Animation t_fall;
	Animation t_spell1;
	Animation t_spell2;
	Animation t_hurt;
	Animation t_death;


	state playerState; 

	Direction dir;

	//audio
	int saveGame;
	int loadGame;
	int atk1SFX;
	int atk2SFX;
	int gJumpSFX;
	int pJumpSFX;
	int pDeathSFX;
	int gDeathSFX;
	int gHurtSFX;
	int pHurtSFX;
	int switchOnSFX;
	int switchOffSFX;
	
	
	
};