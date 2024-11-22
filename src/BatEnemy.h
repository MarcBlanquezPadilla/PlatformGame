#pragma once

#include "Enemy.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"

struct SDL_Texture;

class BatEnemy : public Enemy
{
public:

	BatEnemy();
	~BatEnemy();

	
	bool Start() override;

	bool Update(float dt) override;

	

private:

	int drawOffsetX=0;
	int drawOffsetY=0;

	float speed;
};

