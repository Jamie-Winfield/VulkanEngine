#include "Game.h"
#include <random>

void Game::start()
{
	sprite1 = engine->createSprite("textures/texture.jpg");
	sprite1->setScale(100, 100);
	sprite1->setPos(200, 400, 0);

	sprite2 = engine->createSprite("textures/texture.jpg");
	sprite2->setScale(200, 200);
	sprite2->setPos(400, 400, 0);

	for (int i = 0; i < 400; ++i)
	{
		const int range_from = 0;
		const int range_to = 800;
		std::random_device                  rand_dev;
		std::mt19937                        generator(rand_dev());
		std::uniform_int_distribution<int>  distr(range_from, range_to);
		auto sprite = engine->createSprite("textures/texture.jpg");
		sprite->setScale(10, 10);
		sprite->setPos(distr(generator), distr(generator), 0);
		sprite->setRotation(distr(generator));
		sprites.emplace_back(std::move(sprite));
	}

}

void Game::update(float gameTime)
{
	for (auto &sprite : sprites)
	{
		sprite->setRotation(sprite->getAngle() + 30 * gameTime);
	}
	sprite1->setRotation(sprite1->getAngle() + 30 * gameTime);
	sprite2->setRotation(sprite2->getAngle() - 30 * gameTime);
}

void Game::render(Renderer* renderer)
{
	renderer->renderObject(sprite2.get());
	if (renderSprite)
	{
		renderer->renderObject(sprite1.get());
	}
	for (auto& sprite : sprites)
	{
		renderer->renderObject(sprite.get());
	}
}

void Game::keyHandler(KeyEvent keyEvent)
{
	if (keyEvent.action == KeyEvent::ActionCode::KEY_PRESSED)
	{
		if (keyEvent.key == GLFW_KEY_A)
		{
			renderSprite = true;
		}
		else if (keyEvent.key == GLFW_KEY_D)
		{
			renderSprite = false;
		}
	}
}

void Game::end(VkDevice device)
{
	sprite1->free(device);
	sprite2->free(device);
}
