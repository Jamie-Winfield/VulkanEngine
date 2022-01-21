#pragma once
#include "Engine.h"
class Game
{
public:
	Game(Engine* _engine) : engine(_engine){}

	void start();
	void update(float gameTime);
	void render();
	void keyHandler(int key);
	void end(VkDevice device);

private:
	Engine* engine;
	std::unique_ptr<SpriteObject> sprite1;
	std::unique_ptr<SpriteObject> sprite2;
	std::unique_ptr<SpriteObject> sprite3;
	std::vector<std::unique_ptr<SpriteObject>> spriteObjects;

	bool render3 = false;
};

