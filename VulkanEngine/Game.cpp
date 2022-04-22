#include "Game.h"
#include "Colors.h"
#include <random>

void Game::Start()
{


	atlas = GameHelper::CreateTextureAtlas(1000, 1000, "atlas1");
	atlas->AddImage("textures/texture.jpg");
	atlas->AddImage("textures/texture2.png");



	auto _font = GameHelper::GetFontLoader();
	auto font_id = _font->CreateFont("Fonts/SalmaproMedium-0Wooo.ttf", 25);
	atlas2 = _font->LoadText("qThe quick Brown fox \n Jumped over the red fence", font_id, "atlas2",Colors::Green);


	text = GameHelper::CreateTextObject("qThe quick Brown fox \n Jumped over the red fence", Colors::Red, font_id);
	text->SetPos(500, 400);
	text->SetZOrder(1);

	sprite1 = GameHelper::CreateSprite("atlas1");
	sprite1->SetUVs(atlas->GetUV("textures/texture.jpg"));
	//sprite1->setScale(100, 100);
	sprite1->SetPos(200, 300);

	//sprite2 = engine->createSprite("atlas1");
	//sprite2->setPos(400, 400, 0);

	sprite3 = GameHelper::CreateSprite("atlas2");
	//sprite3->setScale(200, 200);
	sprite3->SetPos(500, 200);
	sprite3->FlipRenderable();

	

	//sprite4 = engine->createSprite("textures/alphatest.png");
	//sprite4->setScale(100, 100);
	//sprite4->setPos(50, 600, 0);

	sprite5 = GameHelper::CreateSprite("textures/texture2.png");
	sprite5->SetPos(190, 200);
	//sprite5->setScale(100, 100);
	
	

	GameHelper::EnableCollisionSystem(true);

	

	collisions = GameHelper::GetCollisionSystem();

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
		auto sprite = GameHelper::CreateSprite("textures/texture.jpg");
		sprite->SetScale(10, 10);
		sprite->SetPos(distr(generator), distr(generator));
		sprite->SetRotation(distr(generator));
		
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
	renderer->renderObject(text);
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
			sprite1->ChangePos(-5, 0);
		}
		else if (keyEvent.key == GLFW_KEY_D)
		{
			sprite1->ChangePos(5, 0);
		}
		else if (keyEvent.key == GLFW_KEY_W)
		{
			sprite1->ChangePos(0, 5);
		}
		else if (keyEvent.key == GLFW_KEY_S)
		{
			sprite1->ChangePos(0, -5);
		}
		
	}
	else if (keyEvent.action == KeyEvent::ActionCode::KEY_PRESSED)
	{
		if (keyEvent.key == GLFW_KEY_ESCAPE)
		{
			GameHelper::StopGame();
		}
		if (keyEvent.key == GLFW_KEY_A)
		{
			sprite1->ChangePos(-5, 0);
		}
		else if (keyEvent.key == GLFW_KEY_D)
		{
			sprite1->ChangePos(5, 0);
		}
		else if (keyEvent.key == GLFW_KEY_W)
		{
			sprite1->ChangePos(0, 5);
		}
		else if (keyEvent.key == GLFW_KEY_S)
		{
			sprite1->ChangePos(0, -5);
		}
		else if (keyEvent.key == GLFW_KEY_Q)
		{
			sprite3->ChangeRotation(-5);
		}
		else if (keyEvent.key == GLFW_KEY_E)
		{
			sprite3->ChangeRotation(5);
		}
		else if (keyEvent.key == GLFW_KEY_R)
		{
			sprite1->SetUVs(atlas->GetUV("textures/texture2.png"));
		}
		else if (keyEvent.key == GLFW_KEY_F)
		{
			collisions->use_threads = !collisions->use_threads;
		}

		if (keyEvent.key == GLFW_KEY_KP_4)
		{
			GameHelper::GetRenderer()->GetCamera()->ChangePosition(-5, 0, 0);
		}
		else if (keyEvent.key == GLFW_KEY_KP_6)
		{
			GameHelper::GetRenderer()->GetCamera()->ChangePosition(5, 0, 0);
		}
		else if (keyEvent.key == GLFW_KEY_KP_8)
		{
			GameHelper::GetRenderer()->GetCamera()->ChangePosition(0, 5, 0);
		}
		else if (keyEvent.key == GLFW_KEY_KP_2)
		{
			GameHelper::GetRenderer()->GetCamera()->ChangePosition(0, -5, 0);
		}
	}
}

void Game::End()
{
}
