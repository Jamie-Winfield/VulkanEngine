#include "Game.h"
#include <random>

void Game::start()
{
	sprite1 = engine->createSprite("textures/texture.jpg");
	


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
}
