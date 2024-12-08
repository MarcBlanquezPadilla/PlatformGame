#include "ParticleManager.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Log.h"
#include "Timer.h"
#include "Player.h"
#include "Entity.h"
#include "Animation.h"
#include "Scene.h"
#include "Physics.h"

ParticleManager:: ParticleManager() : Module()
{
	name = "particleManager";
	for (int i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

ParticleManager::~ParticleManager()
{

}

bool ParticleManager::Awake()
{
	LOG("Loading Particle Manager");
	bool ret = true;

	return ret;

}

bool ParticleManager::Start()
{
	LOG("Loading particles");
	

	pugi::xml_node sceneNode = Engine::GetInstance().scene.get()->configParameters;
	texture = Engine::GetInstance().textures.get()->Load(sceneNode.child("entities").child("shot").attribute("texture").as_string());
	texW = sceneNode.child("entities").child("shot").attribute("w").as_int();
	texH = sceneNode.child("entities").child("shot").attribute("h").as_int();
	
	
	shotRad = sceneNode.child("entities").child("shot").attribute("rad").as_int();
	shot.anim.LoadAnimations(sceneNode.child("entities").child("shot").child("animations").child("travel"));
	
	shot.speed = { 5,0 };
	shot.lifetime = 50;

	return true;
}

bool ParticleManager::Update(float dt)
{
	for (int i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle == nullptr)	continue;
		else if (particle != nullptr && particle->isAlive)
		{
			particle->pbody->body->SetEnabled(true);
			LOG("tex width = %f, tex height = %f", texW, texH);
			particle->pbody->SetPhysPositionWithWorld(particle->position.getX() + texW / 2, particle->position.getY() + texH / 2);
			int posX, posY;
			posX = particle->pbody->GetPhysBodyWorldPosition().getX() - texW / 2;
			posY = particle->pbody->GetPhysBodyWorldPosition().getY() - texH / 2;
			Engine::GetInstance().render.get()->DrawTexture(texture, posX, posY, &(particle->anim.GetCurrentFrame()));
			

		}
		
		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			particle->pbody->body->SetEnabled(false);
			delete particle;
			particles[i] = nullptr;
		}
		
	}

	return true;
}

bool ParticleManager::PostUpdate(float dt)
{
	//Iterating all particle array and drawing any active particles
	for (int i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

	
	}

	return true;
}

bool ParticleManager::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for (int i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

void ParticleManager::AddParticle(const Particle& particle, int x, int y, int delay)
{
	Particle* p = new Particle(particle);

	
	p->frameCount = -(int)delay;			
	p->position.setX(x);
	p->position.setY(y);

	p->pbody = Engine::GetInstance().physics.get()->CreateCircleSensor((int)p->position.getX(), (int)p->position.getY(), shotRad, bodyType::KINEMATIC);
	

	particles[lastParticle++] = p;
	lastParticle %= MAX_ACTIVE_PARTICLES;
}