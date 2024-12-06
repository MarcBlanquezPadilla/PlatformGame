#pragma once


#include "Module.h"
#include "Particle.h"

#define MAX_ACTIVE_PARTICLES 100

struct SDL_Texture;

class ParticleManager : public Module
{
public:

	ParticleManager();


	~ParticleManager();


	bool Start() override;

	bool Awake() override;

	bool Update(float dt) override;


	bool PostUpdate(float dt);


	bool CleanUp() override;


	void AddParticle(const Particle& particle, int x, int y, int delay = 0);

private:

	SDL_Texture* texture;

	Particle* particles[MAX_ACTIVE_PARTICLES] = { nullptr };


	int lastParticle = 0;

public:

	Particle shot;
};
