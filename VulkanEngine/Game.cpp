#include "Game.h"
#include "Colors.h"

void Game::Start()
{
	player1 = GameHelper::CreateSprite("textures/whitepix.png");
	player1->SetScale(50, 200);
	player1->SetPos(0, 300);

	player2 = GameHelper::CreateSprite("textures/whitepix.png");
	player2->SetScale(50, 200);
	player2->SetPos(750, 300);

	top = GameHelper::CreateSprite("textures/whitepix.png");
	top->SetScale(900, 20);
	top->SetPos(0, 780);

	bottom = GameHelper::CreateSprite("textures/whitepix.png");
	bottom->SetScale(900, 20);
	bottom->SetPos(0, 0);

	ball = GameHelper::CreateSprite("textures/whitepix.png");
	ball->SetScale(50, 50);
	ball->SetPos(400, 400);

	font_id = GameHelper::GetFontLoader()->CreateFont("fonts/SalmaproMedium-0Wooo.ttf",30);


	scoreText = GameHelper::CreateTextObject("SCORE", Colors::White, font_id);
	scoreText->SetPos(400, 700);

	p1_score = GameHelper::CreateTextObject("0", Colors::White, font_id);
	p1_score->SetPos(300, 700);

	p2_score = GameHelper::CreateTextObject("0", Colors::White, font_id);
	p2_score->SetPos(450, 700);




	GameHelper::EnableCollisionSystem(true);

	GameHelper::GetCollisionSystem()->CreateBoxCollider(player1);
	GameHelper::GetCollisionSystem()->CreateBoxCollider(player2);
	GameHelper::GetCollisionSystem()->CreateBoxCollider(top);
	GameHelper::GetCollisionSystem()->CreateBoxCollider(bottom);
	GameHelper::GetCollisionSystem()->CreateBoxCollider(ball);

	top->GetBoxCollider()->SetMoveable(false);
	bottom->GetBoxCollider()->SetMoveable(false);
	ball->GetBoxCollider()->SetMoveable(false);
	player1->GetBoxCollider()->SetMoveable(false);
	player2->GetBoxCollider()->SetMoveable(false);
	

	
}

void Game::Update(float gameTime)
{
	if (p1_move_up && player1->GetPosition().y + player1->GetHeight() < 800)
	{
		player1->ChangePos(0, 400 * GameHelper::GameTimeDelta());
	}
	else if (p1_move_down && player1->GetPosition().y > 0)
	{
		player1->ChangePos(0, -400 * GameHelper::GameTimeDelta());
	}
	if (p2_move_up && player2->GetPosition().y + player2->GetHeight() < 800)
	{
		player2->ChangePos(0, 400 * GameHelper::GameTimeDelta());
	}
	else if (p2_move_down && player2->GetPosition().y > 0)
	{
		player2->ChangePos(0, -400 * GameHelper::GameTimeDelta());
	}

	auto events = GameHelper::GetCollisionSystem()->GetCollisionEvents();
	for (auto _event : events)
	{
		if (_event.ContainsBoth(top, ball))
		{
			ball_vector.y = -1;
		}
		else if (_event.ContainsBoth(bottom, ball))
		{
			ball_vector.y = 1;
		}
		else if (_event.ContainsBoth(player1, ball))
		{
			ball_vector.x = 1;
		}
		else if (_event.ContainsBoth(player2, ball))
		{
			ball_vector.x = -1;
		}
	}

	if (ball->GetPosition().x > 800)
	{
		ball->SetPos(400, 400);
		p1Score++;
		GameHelper::ChangeText(p1_score, std::to_string(p1Score),Colors::White,font_id);
	}
	else if (ball->GetPosition().x < 0)
	{
		ball->SetPos(400, 400);
		p2Score++;
		GameHelper::ChangeText(p2_score, std::to_string(p2Score), Colors::White, font_id);
	}


	ball->ChangePos(ball_vector.x * 100 * gameTime, ball_vector.y * 100 * gameTime);
}

void Game::Render(Renderer* renderer)
{
	renderer->renderObject(player1);
	renderer->renderObject(player2);
	renderer->renderObject(top);
	renderer->renderObject(bottom);
	renderer->renderObject(ball);
	renderer->renderObject(scoreText);
	renderer->renderObject(p1_score);
	renderer->renderObject(p2_score);
}

void Game::KeyHandler(KeyEvent keyEvent, Mouse mouse)
{
	if (keyEvent.key == GLFW_KEY_ESCAPE)
	{
		GameHelper::StopGame();
	}
	if (keyEvent.action == KeyEvent::ActionCode::KEY_PRESSED)
	{
		if (keyEvent.key == GLFW_KEY_W)
		{
			p1_move_up = true;
			p1_move_down = false;
		}
		else if (keyEvent.key == GLFW_KEY_S)
		{
			p1_move_up = false;
			p1_move_down = true;
		}

		if (keyEvent.key == GLFW_KEY_UP)
		{
			p2_move_up = true;
			p2_move_down = false;
		}
		else if (keyEvent.key == GLFW_KEY_DOWN)
		{
			p2_move_up = false;
			p2_move_down = true;
		}
	}
	else if (keyEvent.action == KeyEvent::ActionCode::KEY_RELEASED)
	{
		if (keyEvent.key == GLFW_KEY_W)
		{
			p1_move_up = false;
		}
		else if (keyEvent.key == GLFW_KEY_S)
		{
			p1_move_down = false;
		}

		if (keyEvent.key == GLFW_KEY_UP)
		{
			p2_move_up = false;
		}
		else if (keyEvent.key == GLFW_KEY_DOWN)
		{
			p2_move_down = false;
		}
	}
}

void Game::End()
{
}
