#pragma once

#include "SDL2/SDL_rect.h"
#define MAX_FRAMES 25

class Animation
{
public:
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];
	bool loop = true;
	// Allows the animation to keep going back and forth
	bool pingpong = false;

	float currentFrame = 0.0f;
	int totalFrames = 0;
	int loopCount = 0;
	int pingpongDirection = 1;

public:

	void PushBack(const SDL_Rect& rect)
	{
		frames[totalFrames++] = rect;
	}

	void Reset()
	{
		currentFrame = 0;
	}

	bool HasFinished()
	{
		return !loop && !pingpong && loopCount > 0;
	}

	void Update()
	{
		currentFrame += speed;
		if (currentFrame >= totalFrames)
		{
			currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
			++loopCount;

			if (pingpong)
				pingpongDirection = -pingpongDirection;
		}
	}

	const SDL_Rect& GetCurrentFrame() const
	{
		int actualFrame = currentFrame;
		if (pingpongDirection == -1)
			actualFrame = totalFrames - currentFrame;

		return frames[actualFrame];
	}

	void LoadAnimations(pugi::xml_node animationNode)
	{
		speed = animationNode.attribute("speed").as_float();
		loop = animationNode.attribute("loop").as_bool();

		for (pugi::xml_node animation = animationNode.child("frame"); animation; animation = animation.next_sibling("frame"))
		{
			PushBack({ 
				animation.attribute("x").as_int(),
				animation.attribute("y").as_int(),
				animation.attribute("w").as_int(),
				animation.attribute("h").as_int() 
			});
		}

	}
};
