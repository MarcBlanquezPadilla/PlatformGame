#include "Item.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name = "item";
}

Item::~Item() {}

bool Item::Awake() {
	return true;
}

//void Item::SetPlayer(Player* _player)
//{
//	ghost = _player;
//}

bool Item::Start() {

	//initilize textures
	pumpkinTex = Engine::GetInstance().textures.get()->Load("Assets/Textures/Items/pumpkin ui obj-resized.png");
	
	/* L08 TODO 4: Add a physics to an item - initialize the physics body*/
	Engine::GetInstance().textures.get()->GetSize(pumpkinTex, texW, texH);
	position.setX(300);
	position.setY(416);
	texW = 16;
	texH = 16;

	unlit.PushBack({ 0,0,texW,texH});
	lit.PushBack({ 0,texH,texW,texH });


	pbody = Engine::GetInstance().physics.get()->CreateRectangleSensor((int)position.getX() + (texW / 2), (int)position.getY() + (texH / 2), texW / 2, texH/2, bodyType::STATIC);
	
	currentAnim = &lit;

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::ITEM;

	return true;
}

bool Item::Update(float dt)
{
	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  

	/*if (ghost->transformed) {

		currentAnim = &lit;

	}
	else {
		currentAnim = &unlit;
	}*/

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texW / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	
	Engine::GetInstance().render.get()->DrawTexture(pumpkinTex, (int)position.getX(), (int)position.getY(), &currentAnim->GetCurrentFrame());

	return true;
}

bool Item::CleanUp()
{
	return true;
}

