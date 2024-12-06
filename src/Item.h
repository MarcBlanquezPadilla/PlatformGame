#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Player.h"



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
	GREEN,
	PUMPKIN
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

	void SetParameters(pugi::xml_node _parameters) {
		this->parameters = _parameters;
	}

	void SetPlayer(Player* _player);

	void SaveData(pugi::xml_node itemNode);
	void LoadData(pugi::xml_node itemNode);

public:

	bool isPicked = false;

private:
	//L08 TODO 4: Add a physics to an item
	PhysBody* pbody;

	Player* player;

	SDL_Texture* candyTex;
	SDL_Texture* pumpkinTex;

	bool alight;

	const char* texturePath;
	int texW, texH;
	Animation lit;
	Animation unlit;
	Animation* currentAnim;

	pugi::xml_node parameters;
};



