#pragma once
#include "Engine.h"
#include "keyEvent.h"
#include "VulkanGame.h"
class Game : public VulkanGame
{
public:
	/// <summary>
	/// Entrance to the game, creating spites or other initilisation should be preformed here.
	/// </summary>
	void Start() override;

	/// <summary>
	/// Called once per frame and should be used to update sprite positions or rotations
	///  and any other updates needed for the game.
	/// </summary>
	void Update(float gameTime) override;

	/// <summary>
	/// Spites which need to be rendered should be added to the render queue using renderer->renderObject(spriteObject*)
	/// or renderer->renderObject(Text*).
	/// </summary>
	void Render(Renderer* renderer) override;

	/// <summary>
	/// Used to handle key events.
	/// </summary>
	void KeyHandler(KeyEvent keyEvent, Mouse mouse) override;

	/// <summary>
	/// Called at the end of the game and should be used to clean up memory
	/// </summary>
	void End() override;

private:
	SpriteObject* player1 = nullptr;
	bool p1_move_up = false;
	bool p1_move_down = false;

	SpriteObject* player2 = nullptr;
	bool p2_move_up = false;
	bool p2_move_down = false;

	SpriteObject* top = nullptr;
	SpriteObject* bottom = nullptr;

	SpriteObject* ball = nullptr;
	Vector2 ball_vector = Vector2(1, 1);

	Text* scoreText = nullptr;
	Text* p1_score = nullptr;
	int p1Score = 0;
	Text* p2_score = nullptr;
	int p2Score = 0;

	int font_id = 0;
};

