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

private:
	Engine* engine;
	std::unique_ptr<SpriteObject> sprite1;
	std::unique_ptr<SpriteObject> sprite2;
	std::unique_ptr<SpriteObject> sprite3;

	bool render3 = false;
};

