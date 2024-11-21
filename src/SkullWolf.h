#pragma once

#include "Enemy.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"

struct SDL_Texture;

class SkullWolf : public Enemy
{
public:

	SkullWolf();
	~SkullWolf();

	
	bool Start() override;

	bool Update(float dt) override;

	

private:

	int drawOffsetX;
	int drawOffsetY;
};

