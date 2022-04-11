#pragma once
#include "spriteObject.h"
#include <iostream>
#include "ctpl_stl.h"
#include <atomic>
class CollsionSystem
{
public:

	CollsionSystem();

	void AddSprite(SpriteObject* _sprite);
	void RemoveSprite(SpriteObject* _sprite);
	void UpdateCollsions();

	bool debug = false;

	bool use_threads = true;


private:
	std::vector<SpriteObject*> sprites;

	std::vector<SpriteObject*> quad1;
	std::vector<SpriteObject*> quad2;
	std::vector<SpriteObject*> quad3;
	std::vector<SpriteObject*> quad4;

	

	ctpl::thread_pool pool;

};

