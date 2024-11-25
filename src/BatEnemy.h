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

	int drawOffsetX=0;
	int drawOffsetY=0;
	Vector2D destinationTile;
	std::vector<Vector2D> route = { { 200, 350 }, { 400, 350 } };
	


	float speed;
};

