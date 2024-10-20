#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"

struct SDL_Texture;

enum CandyType {
	LOLLY,
	SWIRL,
	CORN,
	HEART,
	SKULL,
	TIME,
	PINK,
	ORANGE,
	YELLOW,
	PURPLE,
	BLUE,
	GREEN
};

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	bool isPicked = false;

private:

	SDL_Texture* candyTex;

	const char* texturePath;
	int texW, texH;

	//L08 TODO 4: Add a physics to an item
	PhysBody* pbody;
};
