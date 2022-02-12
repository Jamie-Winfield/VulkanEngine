#pragma once
#include "Engine.h"
#include "keyEvent.h"
class Game
{
public:
	Game(Engine* _engine) : engine(_engine){}

	/// <summary>
	/// Entrance to the game, creating spites or other initilisation should be preformed here.
	/// </summary>
	void start();

	/// <summary>
	/// Called once per frame and should be used to update sprite positions or rotations
	///  and any other updates needed for the game.
	/// </summary>
	void update(float gameTime);

	/// <summary>
	/// Spites which need to be rendered should be added to the render queue using renderer->renderObject(spriteObject*).
	/// </summary>
	void render(Renderer* renderer);

	/// <summary>
	/// Used to handle key events.
	/// </summary>
	/// <param name="keyEvent"></param>
	void keyHandler(KeyEvent keyEvent);

	/// <summary>
	/// Called at the end of the game and should be used to clean up any spriteObjects using free
	///  and other memory used by the game.
	/// </summary>
	void end(VkDevice device);

private:
	Engine* engine;
	std::unique_ptr<SpriteObject> sprite1;
	std::unique_ptr<SpriteObject> sprite2;
	std::vector<std::unique_ptr<SpriteObject>> sprites;

	bool renderSprite = false;
};

