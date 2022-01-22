#include "Game.h"
#include <random>

void Game::start()
{
	sprite1 = engine->createSprite("textures/texture.jpg");
	sprite1->setScale(100, 100, 1);
	sprite1->setPos(400, 400, 0);
	sprite2 = engine->createSprite("textures/texture2.png");
	sprite2->setScale(100, 100, 1);
	sprite2->setPos(600, 200, 0);

}

void Game::update(float gameTime)
{
	sprite1->setRotation(sprite1->rot_angle += 30 * gameTime);
	for (auto& sprite : spriteObjects)
	{
		sprite->setRotation(sprite->rot_angle += 30 * gameTime);
	}
}

void Game::render()
{
	engine->getRenderer()->renderObject(sprite1.get());
	engine->getRenderer()->renderObject(sprite2.get());
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
}
