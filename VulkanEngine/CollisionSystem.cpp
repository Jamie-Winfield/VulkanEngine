#include "CollisionSystem.h"
#include <thread>

CollisionSystem::CollisionSystem(Vector2 _screen) : screen(_screen)
{
	if (use_threads)
	{
		pool.resize(4);
	}
}


CollisionSystem::BoxColliderErrorCodes CollisionSystem::CreateBoxCollider(SpriteObject* _sprite)
{
	if (_sprite == nullptr)
	{
		return ERROR_SPRITE_IS_NULL;
	}
	for (auto& _collider : box_colliders)
	{
		if (_collider != nullptr && _collider.get() == _sprite->GetBoxCollider())
		{
			return ERROR_BOX_COLLIDER_ALREADY_PRESENT;
		}
	}
	auto _collider = std::make_unique<BoxCollider>(_sprite->GetPos(), _sprite->GetRotation(),
		_sprite->GetScale(), _sprite->GetId(), screen, _sprite);

	box_colliders.emplace_back(std::move(_collider));
	_sprite->SetBoxCollider(box_colliders.back().get());
	return BOX_CREATED;
	
}

void CollisionSystem::DestroyBoxCollider(BoxCollider* _collider)
{
	for (int i = 0; i < box_colliders.size(); ++i)
	{
		if (box_colliders[i].get() == _collider)
		{
			box_colliders.erase(box_colliders.begin() + i);
			_collider = nullptr;
			return;
		}
	}
	_collider = nullptr;
	return;

}


void SolveQuadBox(int id, std::vector<BoxCollider*> colliders)
{
	for (auto& _collider1 : colliders)
	{
		for (auto& _collider2 : colliders)
		{
			if (_collider1 == _collider2)
			{
				continue;
			}

			auto collider1_corners = _collider1->GetCorners();

			float sprite1_x = collider1_corners[0].x;
			if (collider1_corners[1].x < sprite1_x)
			{
				sprite1_x = collider1_corners[1].x;
			}
			if (collider1_corners[2].x < sprite1_x)
			{
				sprite1_x = collider1_corners[2].x;
			}
			if (collider1_corners[3].x < sprite1_x)
			{
				sprite1_x = collider1_corners[3].x;
			}

			float sprite1_x_max = collider1_corners[0].x;
			if (collider1_corners[1].x > sprite1_x_max)
			{
				sprite1_x_max = collider1_corners[1].x;
			}
			if (collider1_corners[2].x > sprite1_x_max)
			{
				sprite1_x_max = collider1_corners[2].x;
			}
			if (collider1_corners[3].x > sprite1_x_max)
			{
				sprite1_x_max = collider1_corners[3].x;
			}

			float sprite1_y = collider1_corners[0].y;
			if (collider1_corners[1].y < sprite1_y)
			{
				sprite1_y = collider1_corners[1].y;
			}
			if (collider1_corners[2].y < sprite1_y)
			{
				sprite1_y = collider1_corners[2].y;
			}
			if (collider1_corners[3].y < sprite1_y)
			{
				sprite1_y = collider1_corners[3].y;
			}

			float sprite1_y_max = collider1_corners[0].y;
			if (collider1_corners[1].y > sprite1_y_max)
			{
				sprite1_y_max = collider1_corners[1].y;
			}
			if (collider1_corners[2].y > sprite1_y_max)
			{
				sprite1_y_max = collider1_corners[2].y;
			}
			if (collider1_corners[3].y > sprite1_y_max)
			{
				sprite1_y_max = collider1_corners[3].y;
			}

			auto collider2_corners = _collider2->GetCorners();

			float sprite2_x = collider2_corners[0].x;
			if (collider2_corners[1].x < sprite2_x)
			{
				sprite2_x = collider2_corners[1].x;
			}
			if (collider2_corners[2].x < sprite2_x)
			{
				sprite2_x = collider2_corners[2].x;
			}
			if (collider2_corners[3].x < sprite2_x)
			{
				sprite2_x = collider2_corners[3].x;
			}

			float sprite2_x_max = collider2_corners[0].x;
			if (collider2_corners[1].x > sprite2_x_max)
			{
				sprite2_x_max = collider2_corners[1].x;
			}
			if (collider2_corners[2].x > sprite2_x_max)
			{
				sprite2_x_max = collider2_corners[2].x;
			}
			if (collider2_corners[3].x > sprite2_x_max)
			{
				sprite2_x_max = collider2_corners[3].x;
			}

			float sprite2_y = collider2_corners[0].y;
			if (collider2_corners[1].y < sprite2_y)
			{
				sprite2_y = collider2_corners[1].y;
			}
			if (collider2_corners[2].y < sprite2_y)
			{
				sprite2_y = collider2_corners[2].y;
			}
			if (collider2_corners[3].y < sprite2_y)
			{
				sprite2_y = collider2_corners[3].y;
			}

			float sprite2_y_max = collider2_corners[0].y;
			if (collider2_corners[1].y > sprite2_y_max)
			{
				sprite2_y_max = collider2_corners[1].y;
			}
			if (collider2_corners[2].y > sprite2_y_max)
			{
				sprite2_y_max = collider2_corners[2].y;
			}
			if (collider2_corners[3].y > sprite2_y_max)
			{
				sprite2_y_max = collider2_corners[3].y;
			}

			if ((sprite1_x >= sprite2_x && sprite1_x <= sprite2_x_max) ||
				(sprite1_x_max >= sprite2_x && sprite1_x_max <= sprite2_x_max))
			{
				// objects overlap on the x axis
				if ((sprite1_y >= sprite2_y && sprite1_y <= sprite2_y_max) ||
					(sprite1_y_max >= sprite2_y && sprite1_y_max <= sprite2_y_max))
				{
					//objects also overlap on the y axis

					std::cout << "objects overlap \n";


					// displace objects by how much they overlap
					SpriteObject* sprite;
					auto _sprite1 = _collider1->GetParent();
					auto _sprite2 = _collider2->GetParent();
					if (_collider1->GetMoveable())
					{
						sprite = _sprite1;
					}
					else
					{
						sprite = _sprite2;
					}
					if (_collider1->GetVector().x > 0)
					{
						//moving right
						float overlap_x = std::abs(sprite1_x_max - sprite2_x) + 1;
						if (_collider2->GetMoveable())
						{
							_sprite2->updatePos(overlap_x, 0, 0);
						}
						if (_collider1->GetMoveable())
						{
							_sprite1->updatePos(-overlap_x, 0, 0);
						}
					}
					else if (_collider1->GetVector().x < 0)
					{
						//moving left
						float overlap_x = std::abs(sprite1_x - sprite2_x_max) + 1;
						if (_collider2->GetMoveable())
						{
							_sprite2->updatePos(-overlap_x, 0, 0);
						}
						if (_collider1->GetMoveable())
						{
							_sprite1->updatePos(overlap_x, 0, 0);
						}
					}
					if (_collider1->GetVector().y > 0)
					{
						//moving up
						float overlap_y = std::abs(sprite1_y_max - sprite2_y) + 1;
						if (_collider2->GetMoveable())
						{
							_sprite2->updatePos(0, overlap_y, 0);
						}
						if (_collider1->GetMoveable())
						{
							_sprite1->updatePos(0, -overlap_y, 0);
						}
					}
					else if (_collider1->GetVector().y < 0)
					{
						//moving down
						float overlap_y = std::abs(sprite1_y - sprite2_y_max) + 1;
						if (_collider2->GetMoveable())
						{
							_sprite2->updatePos(0, -overlap_y, 0);
						}
						if (_collider1->GetMoveable())
						{
							_sprite1->updatePos(0, overlap_y, 0);
						}
					}


				}
			}
		}
	}
}

void CollisionSystem::UpdateCollsions()
{
	if (!use_threads && pool.size() > 0)
	{
		pool.resize(0);
	}
	else if (use_threads && pool.size() <= 0)
	{
		pool.resize(4);
	}


	UpdateBoxColliders();
	

}

void CollisionSystem::UpdateBoxColliders()
{
	boxquad1.clear();
	boxquad2.clear();
	boxquad3.clear();
	boxquad4.clear();

	for (auto& collider : box_colliders)
	{
		switch (collider->quad)
		{
		case BoxCollider::Quad::BOTTOM_LEFT:
		{
			boxquad1.emplace_back(collider.get());
			break;
		}
		case BoxCollider::Quad::BOTTOM_RIGHT:
		{
			boxquad2.emplace_back(collider.get());
			break;
		}
		case BoxCollider::Quad::TOP_LEFT:
		{
			boxquad3.emplace_back(collider.get());
			break;
		}
		case BoxCollider::Quad::TOP_RIGHT:
		{
			boxquad4.emplace_back(collider.get());
			break;
		}
		case BoxCollider::Quad::TOP_LEFT_RIGHT:
		{
			boxquad4.emplace_back(collider.get());
			boxquad3.emplace_back(collider.get());
			break;
		}
		case BoxCollider::Quad::BOTTOM_LEFT_RIGHT:
		{
			boxquad1.emplace_back(collider.get());
			boxquad2.emplace_back(collider.get());
			break;
		}
		case BoxCollider::Quad::LEFT_SIDE:
		{
			boxquad3.emplace_back(collider.get());
			boxquad1.emplace_back(collider.get());
			break;
		}
		case BoxCollider::Quad::RIGHT_SIDE:
		{
			boxquad4.emplace_back(collider.get());
			boxquad2.emplace_back(collider.get());
			break;
		}
		case BoxCollider::Quad::ALL:
		{
			boxquad4.emplace_back(collider.get());
			boxquad3.emplace_back(collider.get());
			boxquad2.emplace_back(collider.get());
			boxquad1.emplace_back(collider.get());
			break;

		}


		}
	}

	if (use_threads)
	{

		pool.push(SolveQuadBox, boxquad1);
		pool.push(SolveQuadBox, boxquad2);
		pool.push(SolveQuadBox, boxquad3);
		pool.push(SolveQuadBox, boxquad4);
		while (pool.n_idle() <= 0)
		{
			// wait for threads
		}
	}
	else
	{


		SolveQuadBox(0, boxquad1);
		SolveQuadBox(0, boxquad2);
		SolveQuadBox(0, boxquad3);
		SolveQuadBox(0, boxquad4);

	}
}






