#pragma once
#include "spriteObject.h"
#include <iostream>
#include <ctpl_stl.h>
#include <atomic>
#include "BoxCollider.h"
class CollisionSystem
{
public:
	enum BoxColliderErrorCodes
	{
		ERROR_BOX_COLLIDER_ALREADY_PRESENT = 1,
		ERROR_SPRITE_IS_NULL = 2,
		BOX_CREATED = 0
	};


	CollisionSystem(Vector2 _screen);

	void AddSprite(SpriteObject* _sprite);
	void RemoveSprite(SpriteObject* _sprite);
	BoxColliderErrorCodes CreateBoxCollider(SpriteObject* _sprite);
	void DestroyBoxCollider(BoxCollider* _collider);
	void UpdateCollsions();

	bool debug = false;

	bool use_threads = false;

	bool wait_threads = true;


private:

	void UpdateBoxColliders();

	std::vector<SpriteObject*> sprites;

	std::vector<SpriteObject*> quad1;
	std::vector<SpriteObject*> quad2;
	std::vector<SpriteObject*> quad3;
	std::vector<SpriteObject*> quad4;

	std::vector<std::unique_ptr<BoxCollider>> box_colliders;

	std::vector<BoxCollider*> boxquad1;
	std::vector<BoxCollider*> boxquad2;
	std::vector<BoxCollider*> boxquad3;
	std::vector<BoxCollider*> boxquad4;

	Vector2 screen;


	

	ctpl::thread_pool pool;

};

