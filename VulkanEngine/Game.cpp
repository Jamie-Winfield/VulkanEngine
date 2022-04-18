#include "Game.h"

#include <random>

void Game::Start()
{


	atlas = std::make_unique<TextureAtlas>(1000, 1000, engine, "atlas1");
	atlas->AddImage("textures/texture.jpg");
	atlas->AddImage("textures/texture2.png");

	font = std::make_unique<LoadFont>(engine);
	auto font_id = font->CreateFont("Fonts/SalmaproMedium-0Wooo.ttf", 25);
	atlas2 = font->LoadText("The quick Brown fox \n Jumped over the red fence", font_id, "atlas2");


	sprite1 = engine->createSprite("atlas1");
	sprite1->ChangeUVs(atlas->GetUV("textures/texture.jpg"), engine->getDevice(),
		engine->getPhysicalDevice(), engine->getRenderer()->GetCommandPool(), engine->getGraphicsQueue());
	//sprite1->setScale(100, 100);
	sprite1->setPos(200, 300, 0);

	//sprite2 = engine->createSprite("atlas1");
	//sprite2->setPos(400, 400, 0);

	sprite3 = engine->createSprite("atlas2");
	//sprite3->setScale(200, 200);
	sprite3->setPos(500, 200, 1);
	sprite3->FlipSprite();

	

	//sprite4 = engine->createSprite("textures/alphatest.png");
	//sprite4->setScale(100, 100);
	//sprite4->setPos(50, 600, 0);

	sprite5 = engine->createSprite("textures/texture2.png");
	sprite5->setPos(190, 200, 0);
	//sprite5->setScale(100, 100);
	
	

	engine->EnableCollisionSystem();

	

	collisions = engine->GetCollisionSystem();

	//collisions->CreateBoxCollider(sprite1);
	//collisions->CreateBoxCollider(sprite2);
	//collisions->CreateBoxCollider(sprite3);
	//collisions->CreateBoxCollider(sprite4);
	//collisions->CreateBoxCollider(sprite5);
	//sprite5->GetBoxCollider()->SetMoveable(false);


	
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

void Game::Update(float gameTime)
{
}

void Game::Render(Renderer* renderer)
{
	renderer->renderObject(sprite1);
	//renderer->renderObject(sprite2);
	renderer->renderObject(sprite3);
	//renderer->renderObject(sprite4);
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
	if (keyEvent.key == GLFW_MOUSE_BUTTON_LEFT && keyEvent.action == GLFW_PRESS)
	{
		std::cout << "mouse left pressed!" << "\n";
	}

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
			sprite3->Rotate(-5);
		}
		else if (keyEvent.key == GLFW_KEY_E)
		{
			sprite3->Rotate(5);
		}
		else if (keyEvent.key == GLFW_KEY_R)
		{
			sprite1->ChangeUVs(atlas->GetUV("textures/texture2.png"), engine->getDevice(),
				engine->getPhysicalDevice(), engine->getRenderer()->GetCommandPool(), engine->getGraphicsQueue());
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
