#include "GuiControlButton.h"
#include "Render.h"
#include "Engine.h"
#include "Audio.h"
#include "Input.h"

GuiControlButton::GuiControlButton(int id, SDL_Rect bounds, const char* text, SDL_Texture* btTex) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	
	this->texture = btTex;
	/*this->font = _font;*/

	canClick = true;
	drawBasic = false;
	this->active = true;
	state = GuiControlState::NORMAL;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	/*if (state != GuiControlState::DISABLED)*/
	
	// L16: TODO 3: Update the state of the GUiButton according to the mouse position
	if (state != GuiControlState::DISABLED) {
		Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();

		//If the position of the mouse if inside the bounds of the button 
		if (mousePos.getX() > bounds.x && mousePos.getX() < bounds.x + bounds.w && mousePos.getY() > bounds.y && mousePos.getY() < bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}
	

	//L16: TODO 4: Draw the button according the GuiControl State
	switch (state)
	{
	case GuiControlState::DISABLED:
		section = {0, 32, 192, 32};
		break;
	case GuiControlState::NORMAL:
		section = {0, 0, 192, 32};
		break;
	case GuiControlState::FOCUSED:
		section = {0, 64, 192, 32};
		break;
	case GuiControlState::PRESSED:
		section = {0, 96, 192, 32};
		break;
	}

	if (active) {
		if (texture != nullptr) {
			Engine::GetInstance().render.get()->DrawTexture(texture, bounds.x, bounds.y, &section);
		}

		Engine::GetInstance().render->DrawText(text.c_str(), bounds.x * 2 + bounds.w / 2, bounds.y * 2 + bounds.h / 2, bounds.w, bounds.h);
	}
	
		
	

	return false;
}