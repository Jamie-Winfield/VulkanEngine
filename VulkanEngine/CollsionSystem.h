#pragma once
#include "spriteObject.h"
#include <iostream>
class CollsionSystem
{
public:
	void AddSprite(SpriteObject* _sprite);
	void RemoveSprite(SpriteObject* _sprite);
	void UpdateCollsions();

	bool debug = false;


private:
	std::vector<SpriteObject*> sprites;

	std::vector<SpriteObject*> quad1;
	std::vector<SpriteObject*> quad2;
	std::vector<SpriteObject*> quad3;
	std::vector<SpriteObject*> quad4;

	/*
	void SolveQuad1();
	void SolveQuad2();
	void SolveQuad3();
	void SolveQuad4();
	*/

};

