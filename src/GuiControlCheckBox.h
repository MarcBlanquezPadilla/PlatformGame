#pragma once

#include "GuiControl.h"
#include "Vector2D.h"

class GuiControlCheckBox : public GuiControl
{

public:

	GuiControlCheckBox(const char* name, SDL_Rect bounds, const char* text, SDL_Texture* tex);
	virtual ~GuiControlCheckBox();

	// Called each loop iteration
	bool Update(float dt);

private:

	TTF_Font* font;
	bool isChecked = false;

};
