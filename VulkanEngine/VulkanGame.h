#pragma once
#include "keyEvent.h"
#include "Mouse.h"
#include "GameHelper.h"

class VulkanGame
{
	virtual void Start() = 0;

	virtual void Update(float gameTime) = 0;

	virtual void Render(Renderer* renderer) = 0;

	virtual void KeyHandler(KeyEvent keyEvent, Mouse mouse) = 0;

	virtual void End() = 0;
};
