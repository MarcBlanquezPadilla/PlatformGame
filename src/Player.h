#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "Timer.h"

#define GHOST_W 9

struct SDL_Texture;

enum state {
	IDLE,
	WALK,
	JUMP,
	FALL,
	HURT,
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

public:

	//Declare player parameters
	float isWalking;
	SDL_Texture* texture;
	int texW, texH;
	SDL_Rect currentFrame;


	bool menu;
	SDL_Texture* helpMenu;
	int helpMenuX, helpMenuY, helpMenuW, helpMenuH;
	int cameraX, cameraY, cameraW, cameraH;

	//Audio fx
	int pickCoinFxId;

	// L08 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	float jumpForce = 0.6f; // The force to apply when jumping
	float pushForce = 0.5f; // The force to apply when getting pushed;
	float moveSpeed = 0.2f;
	float friction = 2.0f;
	float gravity = 1.2f;

	
	bool destroyed = false;
	bool godMode = false;
	bool tpToStart = false;
	bool canClimb = false;

	Timer hurtTimer;
	float hurtTime = 1;
	
	Timer respawnTimer;
	float respawnTime = 2.0f;

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


	state playerState = IDLE; 
	state previousState = IDLE;

	Direction dir = RIGHT;
	
};