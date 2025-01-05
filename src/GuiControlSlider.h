#pragma once
#include "GuiControl.h"

#include "Vector2D.h"
#include <string.h>

class GuiControlSlider : public GuiControl
{
public:

    GuiControlSlider(const char* name, const char* text, SDL_Rect bounds, SDL_Rect sliderBounds, SDL_Texture* tex, SDL_Texture* sliderTex);
    virtual ~GuiControlSlider();

    bool Update(float dt);
    /*bool Draw();*/

    float value = 90;
    float value2 = 90;
   
    
    SDL_Rect sliderBounds;
    SDL_Texture* sliderTexture;

    float unit, sliderPosX;

private:

    // GuiControlSlider specific properties
    // Maybe some animation properties for state change?
    GuiControlId id;
    int minValue;
    int maxValue;

    /*bool playFxOnce = true;*/
};


