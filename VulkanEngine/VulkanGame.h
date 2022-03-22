#pragma once
#include "Engine.h"
#include "keyEvent.h"
class VulkanGame
{
	virtual void Start() = 0;

	virtual void Update(float gameTime) = 0;

	virtual void Render(Renderer* renderer) = 0;

	virtual void KeyHandler(KeyEvent keyEvent) = 0;

	virtual void End() = 0;

protected:
	Engine* engine;
};

