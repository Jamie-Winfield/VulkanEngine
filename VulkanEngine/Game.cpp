#include "Game.h"

void Game::start()
{
	sprite1 = engine->createSprite();
	sprite2 = engine->createSprite();
	sprite3 = engine->createSprite();

	sprite1->setPos(400, 300, 0);
	sprite1->setScale(200, 200, 1);
	sprite2->setPos(0, 0, 0);
	sprite3->setPos(0, 1, 0);


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

void Game::end(VkDevice device)
{
	sprite1->free(device);
	sprite2->free(device);
	sprite3->free(device);
}
