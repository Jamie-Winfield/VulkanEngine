#include "Game.h"

void Game::start()
{
	sprite1 = engine->createSprite();
	sprite2 = engine->createSprite();
	sprite3 = engine->createSprite();

	sprite1->setPos(1, 1, 0);
	sprite2->setPos(-1, 0, 0);
	sprite3->setPos(-1, 1, 0);
}

void Game::update(float gameTime)
{
}

void Game::render()
{
	engine->getRenderer()->renderObject(sprite1.get());
	engine->getRenderer()->renderObject(sprite2.get());
	if (render3)
	{
		engine->getRenderer()->renderObject(sprite3.get());
	}
}

void Game::keyHandler(int key)
{
	if (key == GLFW_KEY_A)
	{
		render3 = true;
	}
	else if (key == GLFW_KEY_D)
	{
		render3 = false;
	}
}
