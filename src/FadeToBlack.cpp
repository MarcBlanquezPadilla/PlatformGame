#include "FadeToBlack.h"

#include "Engine.h"
#include "Render.h"
#include "Window.h"
#include "Log.h"

#include "SDL2/SDL_render.h"

class Window;

FadeToBlack::FadeToBlack(bool startEnabled) : Module(startEnabled)
{
	//int windowW, windowH;
	//Engine::GetInstance().window.get()->GetWindowSize(windowW, windowH);
	//int scale = Engine::GetInstance().window.get()->GetScale();
	name = "fade to black";
	screenRect = { 0, 0, 1280 , 640 };
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(Engine::GetInstance().render.get()->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update(float dt)
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			// TODO 1: Enable / disable the modules received when FadeToBlacks() gets called
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NONE;
		}
	}

	return true;
}

bool FadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(Engine::GetInstance().render.get()->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(Engine::GetInstance().render.get()->renderer, &screenRect);

	return true;
}

bool FadeToBlack::Fade(Module* moduleToDisable, Module* moduleToEnable, float frames)
{
	bool ret = false;

	if (currentStep == Fade_Step::NONE)
	{
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}

	return ret;
}