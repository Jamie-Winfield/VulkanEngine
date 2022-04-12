#pragma once
#include "spriteObject.h"
#include <iostream>
#include "ctpl_stl.h"
#include <atomic>
#include "BoxCollider.h"
class CollisionSystem
{
public:

	CollisionSystem();

	void AddSprite(SpriteObject* _sprite);
	void RemoveSprite(SpriteObject* _sprite);
	void AddBoxCollider(BoxCollider* _collider);
	void UpdateCollsions();

	bool debug = false;

	bool use_threads = true;

	bool wait_threads = true;

	bool use_sprites = true;


private:

	void UpdateBoxColliders();

	std::vector<SpriteObject*> sprites;

	std::vector<SpriteObject*> quad1;
	std::vector<SpriteObject*> quad2;
	std::vector<SpriteObject*> quad3;
	std::vector<SpriteObject*> quad4;

	std::vector<BoxCollider*> box_colliders;

	std::vector<BoxCollider> boxquad1;
	std::vector<BoxCollider> boxquad2;
	std::vector<BoxCollider> boxquad3;
	std::vector<BoxCollider> boxquad4;


	

	ctpl::thread_pool pool;

};

