#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"

#define GHOST_W 9


struct SDL_Texture;

enum direction {
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

public:

	//Declare player parameters
	float speed = 5.0f;
	float isWalking = false;
	SDL_Texture* texture = NULL;
	int texW, texH;
	SDL_Rect currentFrame;

	//Audio fx
	int pickCoinFxId;

	// L08 TODO 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	float jumpForce = 0.4f; // The force to apply when jumping
	bool isJumping = false; // Flag to check if the player is currently jumping
	bool destroyed = false;

	Animation* currentAnim = nullptr;

	Animation idle;
	Animation walk;
	Animation jump;
	Animation hurt;
	Animation death;

	direction dir = RIGHT;
	
};