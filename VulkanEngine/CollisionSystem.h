#pragma once
#include "spriteObject.h"
#include <iostream>
#include <ctpl_stl.h>
#include <atomic>
#include "BoxCollider.h"
#include "CollisionEvent.h"
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

	std::vector<CollisionEvent> GetCollisionEvents();

	
	BoxColliderErrorCodes CreateBoxCollider(SpriteObject* _sprite);
	void DestroyBoxCollider(BoxCollider* _collider);
	void UpdateCollsions();

	bool debug = false;

	bool use_threads = false;

	bool wait_threads = true;


private:

	void UpdateBoxColliders();

	void ClearDuplicateEvents();
	

	std::vector<std::unique_ptr<BoxCollider>> box_colliders;

	std::vector<BoxCollider*> boxquad1;
	std::vector<BoxCollider*> boxquad2;
	std::vector<BoxCollider*> boxquad3;
	std::vector<BoxCollider*> boxquad4;

	std::vector<CollisionEvent> quad1_events;
	std::vector<CollisionEvent> quad2_events;
	std::vector<CollisionEvent> quad3_events;
	std::vector<CollisionEvent> quad4_events;

	Vector2 screen;

	std::vector<CollisionEvent> collision_events;
	

	ctpl::thread_pool pool;

};

