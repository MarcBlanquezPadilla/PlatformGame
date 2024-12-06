#pragma once
#pragma once

#include "Enemy.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include <vector>
#include "Timer.h"



struct SDL_Texture;

class GroundEnemy : public Enemy
{
public:

	GroundEnemy();
	~GroundEnemy();


	bool Start() override;

	bool Update(float dt) override;


	
private:

	int drawOffsetX = 0;
	int drawOffsetY = 0;
	Vector2D destinationPoint;
	Animation walk;
	float attackTime;
	Timer attackTimer;

	bool canAttack;
	bool attacked;

	float dist;

	float jumpForce;
};

