#pragma once

#include "Vector2D.h"
#include "Entity.h"
#include "Physics.h"

class Particle : public Entity
{
public:
	// Constructor
	Particle();

	// Copy constructor
	/*Particle(const Particle& p);*/
	bool Start();


	bool Update();

	//bool CleanUp();

public:

	PhysBody* pbody;


	Vector2D position;
	Vector2D speed;
	Animation anim;


	bool isAlive = false;

	// Defines the amout of frames this particle has been active
	// Negative values mean the particle is waiting to be activated
	int frameCount;

	//Should be uint
	int lifetime;
};


