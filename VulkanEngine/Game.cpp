#include "Game.h"
#include <random>

void Game::Start()
{
	sprite1 = engine->createSprite("textures/texture.jpg");
	sprite1->setScale(100, 100);
	sprite1->setPos(200, 200, 0);
	sprite1->Rotate(90);

	sprite2 = engine->createSprite("textures/texture2.jpg");
	sprite2->setScale(200, 200);
	sprite2->setPos(400, 400, 0);

	sprite3 = engine->createSprite("textures/texture.jpg");
	sprite3->setScale(100, 100);
	sprite3->setPos(650, 650, 0);

	sprite4 = engine->createSprite("textures/texture.jpg");
	sprite4->setScale(100, 100);
	sprite4->setPos(100, 100, 0);

	sprite5 = engine->createSprite("textures/texture2.png");
	sprite5->setScale(100, 100);
	sprite5->setPos(350, 100, 0);
	sprite5->moveable = false;

	collisions = std::make_unique<CollisionSystem>();
	collisions->AddSprite(sprite1);
	collisions->AddSprite(sprite2);
	collisions->AddSprite(sprite3);
	collisions->AddSprite(sprite4);
	collisions->AddSprite(sprite5);
	collisions->AddBoxCollider(sprite1->GetBoxCollider());
	collisions->AddBoxCollider(sprite2->GetBoxCollider());
	collisions->AddBoxCollider(sprite3->GetBoxCollider());
	collisions->AddBoxCollider(sprite4->GetBoxCollider());
	collisions->AddBoxCollider(sprite5->GetBoxCollider());
	
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
		//collisions->AddSprite(sprite);
		
		sprites.emplace_back(std::move(sprite));
	}
	

}

void Game::Update(float gameTime)
{
	collisions->UpdateCollsions();
}

void Game::Render(Renderer* renderer)
{
	renderer->renderObject(sprite2);
	renderer->renderObject(sprite1);
	renderer->renderObject(sprite3);
	renderer->renderObject(sprite4);
	renderer->renderObject(sprite5);
	if (flip)
	{
		for (auto sprite : sprites)
		{
			renderer->renderObject(sprite);
		}
	}
	
}

void Game::KeyHandler(KeyEvent keyEvent, Mouse mouse)
{
	if (keyEvent.action == KeyEvent::ActionCode::KEY_HELD)
	{
		if (keyEvent.key == GLFW_KEY_A)
		{
			sprite1->updatePos(-5, 0, 0);
		}
		else if (keyEvent.key == GLFW_KEY_D)
		{
			sprite1->updatePos(5, 0, 0);
		}
		else if (keyEvent.key == GLFW_KEY_W)
		{
			sprite1->updatePos(0, 5, 0);
		}
		else if (keyEvent.key == GLFW_KEY_S)
		{
			sprite1->updatePos(0, -5, 0);
		}
		
	}
	else if (keyEvent.action == KeyEvent::ActionCode::KEY_PRESSED)
	{
		if (keyEvent.key == GLFW_KEY_ESCAPE)
		{
			engine->StopGame();
		}
		if (keyEvent.key == GLFW_KEY_A)
		{
			sprite1->updatePos(-5, 0, 0);
		}
		else if (keyEvent.key == GLFW_KEY_D)
		{
			sprite1->updatePos(5, 0, 0);
		}
		else if (keyEvent.key == GLFW_KEY_W)
		{
			sprite1->updatePos(0, 5, 0);
		}
		else if (keyEvent.key == GLFW_KEY_S)
		{
			sprite1->updatePos(0, -5, 0);
		}
		else if (keyEvent.key == GLFW_KEY_Q)
		{
			sprite1->Rotate(-5);
		}
		else if (keyEvent.key == GLFW_KEY_E)
		{
			sprite1->Rotate(5);
		}
		else if (keyEvent.key == GLFW_KEY_R)
		{
			collisions->wait_threads = !collisions->wait_threads;
		}
		else if (keyEvent.key == GLFW_KEY_F)
		{
			collisions->use_threads = !collisions->use_threads;
		}

		if (keyEvent.key == GLFW_KEY_KP_4)
		{
			engine->getRenderer()->GetCamera()->ChangePosition(-5, 0, 0);
		}
		else if (keyEvent.key == GLFW_KEY_KP_6)
		{
			engine->getRenderer()->GetCamera()->ChangePosition(5, 0, 0);
		}
		else if (keyEvent.key == GLFW_KEY_KP_8)
		{
			engine->getRenderer()->GetCamera()->ChangePosition(0, 5, 0);
		}
		else if (keyEvent.key == GLFW_KEY_KP_2)
		{
			engine->getRenderer()->GetCamera()->ChangePosition(0, -5, 0);
		}
	}
}

void Game::End()
{
}
