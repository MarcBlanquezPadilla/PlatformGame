#pragma once
#pragma once

#include "Enemy.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include <vector>



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
	int routeDestinationIndex;
	std::vector<Vector2D> route = { { 100, 420 }, { 275, 390 } };
	Animation walk;
	Animation attack;





	float speed;
	float jumpForce;
};

