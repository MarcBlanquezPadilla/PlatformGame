#include "Particle.h"
#include "Timer.h"
#include "Engine.h"
#include "Textures.h"



Particle::Particle() : Entity(EntityType::SHOT)
{
}

bool Particle::Start() {

	
	return true;
}

bool Particle::Update()
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the delay has been reached
	if (!isAlive && frameCount >= 0)
		isAlive = true;

	if (isAlive)
	{
		anim.Update();

		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifetime > 0)
		{
			if (frameCount >= lifetime)
				ret = false;
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (anim.HasFinished())
			ret = false;

		// Update the position in the screen
		position.setX(position.getX() + speed.getX());
		position.setY(position.getY() + speed.getY());

		
	}

	return ret;
}