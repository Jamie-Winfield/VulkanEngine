#include "Game.h"
#include <random>

void Game::start()
{
	sprite1 = engine->createSprite("textures/texture.jpg");
	sprite1->setScale(100, 100);
	sprite1->setPos(400, 400, 0);

	

}

void Game::update(float gameTime)
{
	sprite1->setRotation(sprite1->getAngle() + 30 * gameTime);
}

void Game::render(Renderer* renderer)
{
	if (renderSprite)
	{
		renderer->renderObject(sprite1.get());
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
}
