#pragma once


#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"

class Player;

struct SDL_Texture;

class Enemy : public Entity
{
public:
	enum Type {
		FLY,
		GROUND
	};

	enum State {
		PATROL,
		CHASING
	};

	enum Dir {
		LEFT,
		RIGHT
	};

	Enemy();
	virtual ~Enemy();

	bool Awake();

	virtual bool Start();

	virtual bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	void ResetPath();

	void AddAnimation(Animation& anim, int startPosY, int frameSize, int numFrames);

	bool CheckIfTwoPointsNear(Vector2D point1, Vector2D point2, float nearDistance);

	void SetPlayer(Player* player);

	virtual void SaveData(pugi::xml_node enemyNode);

	virtual void LoadData(pugi::xml_node enemyNode);

public:

//protected:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;
	PhysBody* pbody;
	Player* player;

	State state;
	Dir dir;
	Pathfinding* pathfinding;
	int chaseArea;
};
