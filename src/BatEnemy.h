#pragma once

#include "Enemy.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include <vector>



struct SDL_Texture;

class BatEnemy : public Enemy
{
public:

	BatEnemy();
	~BatEnemy();

	
	bool Start() override;

	bool Update(float dt) override;

	
	

private:

	int drawOffsetX=-16;
	int drawOffsetY=-16;
	Vector2D destinationPoint;
	
	
};

