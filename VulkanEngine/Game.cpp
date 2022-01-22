#include "Game.h"
#include <random>

void Game::start()
{
	sprite1 = engine->createSprite();
	sprite2 = engine->createSprite();
	sprite3 = engine->createSprite();

	sprite1->setPos(400, 300, 0);
	sprite1->setScale(200, 200, 1);
	sprite2->setPos(0, 0, 0);
	sprite3->setPos(100, 100, 0);
	sprite3->setScale(200, 200, 1);

	for (int i = 0; i < 200; ++i)
	{
		const int range_from = 0;
		const int range_to = 800;
		std::random_device                  rand_dev;
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr(range_from, range_to);
		auto sprite = engine->createSprite();
		sprite->setScale(10, 10, 1);
		sprite->setPos(distr(generator), distr(generator), 0);
		sprite->setRotation(distr(generator));
		spriteObjects.emplace_back(std::move(sprite));
	}


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
	if (render3)
	{
		engine->getRenderer()->renderObject(sprite3.get());
	}
	for (auto& sprite : spriteObjects)
	{
		engine->getRenderer()->renderObject(sprite.get());
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
