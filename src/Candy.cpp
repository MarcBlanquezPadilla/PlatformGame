#include "Candy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"


Candy::Candy() : Entity(EntityType::ITEM)
{
	name = "item";
}

Candy::~Candy() {}

bool Candy::Awake() {
	return true;
}


bool Candy::Start() {

	pugi::xml_node baseParameters = Engine::GetInstance().scene.get()->configParameters.child("entities").child("items").child("pumpkins");
	//initilize textures
	pumpkinTex = Engine::GetInstance().textures.get()->Load(baseParameters.child("properties").attribute("texture").as_string());

	/* L08 TODO 4: Add a physics to an item - initialize the physics body*/

	position.setX(parameters.attribute("x").as_float());
	position.setY(parameters.attribute("y").as_float());

	texW = baseParameters.child("properties").attribute("w").as_float();
	texH = baseParameters.child("properties").attribute("h").as_float();


	pbody = Engine::GetInstance().physics.get()->CreateRectangleSensor((int)position.getX() + texW / 2, (int)position.getY() + texH / 2, texW, texH, bodyType::STATIC);


	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::ITEM;

	return true;
}

bool Candy::Update(float dt)
{
	if (!Engine::GetInstance().render.get()->InCameraView(pbody->GetPosition().getX() - texW, pbody->GetPosition().getY() - texH, texW, texH))
	{
		return true;
	}

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	
	Engine::GetInstance().render.get()->DrawTexture(pumpkinTex, (int)position.getX(), (int)position.getY(), &currentAnim->GetCurrentFrame());

	return true;
}

bool Candy::CleanUp()
{
	return true;
}

void Candy::SetPlayer(Player* _player) {
	player = _player;
}

void Candy::SaveData(pugi::xml_node itemNode)
{
	itemNode.attribute("alight").set_value(true);
	itemNode.attribute("x").set_value(pbody->GetPhysBodyWorldPosition().getX());
	itemNode.attribute("y").set_value(pbody->GetPhysBodyWorldPosition().getY());
}


void Candy::LoadData(pugi::xml_node itemNode)
{
	pbody->SetPhysPositionWithWorld(itemNode.attribute("x").as_float(), itemNode.attribute("y").as_float());

}
