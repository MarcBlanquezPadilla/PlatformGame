#include "Candy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"


Candy::Candy() : Entity(EntityType::CANDY)
{
	name = "candy";
}

Candy::~Candy() {}

bool Candy::Awake() {
	return true;
}


bool Candy::Start() {

	pugi::xml_node baseParameters = Engine::GetInstance().scene.get()->configParameters.child("entities").child("items").child("candies");
	//initilize textures
	candyTex = Engine::GetInstance().textures.get()->Load(baseParameters.child("properties").attribute("texture").as_string());

	texW = baseParameters.child("properties").attribute("w").as_float();
	texH = baseParameters.child("properties").attribute("h").as_float();
	/* L08 TODO 4: Add a physics to an item - initialize the physics body*/

	position.setX(parameters.attribute("x").as_float());
	position.setY(parameters.attribute("y").as_float());
	type = parameters.attribute("type").as_string();

	function = POINTS;
	if (type == "corn") yAnim = 0;
	else if (type == "ube") yAnim = 16;
	else if (type == "swirl") yAnim = 32;
	else if (type == "toffee") yAnim = 48;
	else if (type == "mint") yAnim = 64;
	else if (type == "lolly") yAnim = 80;
	else if (type == "grape") yAnim = 96;
	else if (type == "choc") yAnim = 112;
	else if (type == "clock") {
		yAnim = 128;
		function = TIME;
	}
	else if (type == "heart")
	{
		yAnim = 144;
		function = HEALING;
	}

	
	/*pugi::xml_node floatAnimNode = baseParameters.child("animations").child("floating").first_child();
	for (floatAnimNode; floatAnimNode != nullptr; floatAnimNode = floatAnimNode.next_sibling()) {
		floatAnimNode.
		
	}*/
	/*LOG("yAnim = %d", floatAnimNode.attribute("y").as_int());*/
	floating.LoadAnimations(baseParameters.child("animations").child("floating"));

	for (int i = 0; i < floating.totalFrames; ++i)
		floating.frames[i].y = yAnim;


	pbody = Engine::GetInstance().physics.get()->CreateRectangleSensor((int)position.getX() + texW / 2, (int)position.getY() + texH / 2, texW, texH, bodyType::STATIC);

	
	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::CANDY;

	return true;
}

bool Candy::Update(float dt)
{
	if (!Engine::GetInstance().render.get()->InCameraView(pbody->GetPosition().getX() - texW, pbody->GetPosition().getY() - texH, texW, texH))
	{
		return true;
	}

	if (player->pickedItem && pbody->body->IsEnabled()) {
		if (function == POINTS) {
			player->candyNum++;
			player->candySFX = player->eatCandySFX;
		}
		else if (function == HEALING) {
			player->lives++;
			player->candySFX = player->loadGameSFX;
		}
		Engine::GetInstance().audio.get()->PlayFx(player->candySFX);
		pbody->body->SetEnabled(false);
		player->pickedItem = false;
		picked = true;
	}
	else {
		currentAnim = &floating;
	}

	if (!picked) {
		b2Transform pbodyPos = pbody->body->GetTransform();
		position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2);
		position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

		currentAnim->Update();
		Engine::GetInstance().render.get()->DrawTexture(candyTex, (int)position.getX(), (int)position.getY(), &currentAnim->GetCurrentFrame());
	}
	
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
