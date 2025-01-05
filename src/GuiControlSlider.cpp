#include "GuiControlSlider.h"
#include "Engine.h"
#include "Scene.h"
#include "GuiManager.h"
#include "GuiControl.h"
#include "Audio.h"
#include "Window.h"
#include "MainMenu.h"

//GuiControlButton::GuiControlButton(const char* name, SDL_Rect bounds, const char* text, SDL_Texture* btTex) : GuiControl(GuiControlType::BUTTON, id, name)
GuiControlSlider::GuiControlSlider(const char* name, const char* text, SDL_Rect circleBounds, SDL_Rect barBounds, SDL_Texture* circleTex, SDL_Texture* barTex) : GuiControl(GuiControlType::SLIDER, id, name)
{
    this->bounds = circleBounds;
    this->sliderBounds = barBounds;
    this->unit = bounds.w / 100.0f;
    this->texture = circleTex;
    this->sliderTexture = barTex;

    value = round(value);
    this->active = false;
    sliderPosX = ((value * unit) + bounds.x) - unit;
}

GuiControlSlider::~GuiControlSlider()
{
}

bool GuiControlSlider::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            state = GuiControlState::FOCUSED;

            unit = bounds.w / 100.0f;
            value = (mouseX - bounds.x) / unit;
            value = round(value);


            value2 = (sliderPosX - bounds.x) / unit;
            value2 = round(value2);


            if (value2 < 2)
                value2 = 0;

            if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                sliderPosX = ((value * unit) + bounds.x) - unit - 5;
            }

            // If mouse button pressed -> Generate event!
            if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                state = GuiControlState::SELECTED;
            }
        }
        else state = GuiControlState::NORMAL;

        

        return false;
    }
}

