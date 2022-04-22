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
	/// Spites which need to be rendered should be added to the render queue using renderer->renderObject(spriteObject*).
	/// </summary>
	void Render(Renderer* renderer) override;

	/// <summary>
	/// Used to handle key events.
	/// </summary>
	/// <param name="keyEvent"></param>
	void KeyHandler(KeyEvent keyEvent, Mouse mouse) override;

	/// <summary>
	/// Called at the end of the game and should be used to clean up any spriteObjects using free
	///  and other memory used by the game.
	/// </summary>
	void End() override;

private:
	SpriteObject* sprite1;
	SpriteObject* sprite2;
	SpriteObject* sprite3;
	SpriteObject* sprite4;
	SpriteObject* sprite5;
	std::vector<SpriteObject*> sprites;
	CollisionSystem* collisions;

	TextureAtlas* atlas;
	std::unique_ptr<TextureAtlas> atlas2;
	std::unique_ptr<LoadFont> font;


	Text* text;

	bool flip = false;

	bool renderSprite = false;
};

