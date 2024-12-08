#include "Particle.h"
#include "Timer.h"
#include "Engine.h"
#include "Textures.h"
#include "log.h"
#include "Scene.h"



Particle::Particle() : Entity(EntityType::SHOT)
{
}

bool Particle::Start() {

	bool isAlive = false;
	bool hitEnemy = false;
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


void Particle::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::ENEMY:
		LOG("Collision ENEMY");
		hitEnemy = true;
		Engine::GetInstance().physics.get()->DeletePhysBody(physB);
		/*pbody->body->SetEnabled(false);*/
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;

	default:
		break;
	}
}