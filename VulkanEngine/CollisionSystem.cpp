#include "CollisionSystem.h"
#include <thread>

CollisionSystem::CollisionSystem()
{
	if (use_threads)
	{
		pool.resize(8);
	}
}

void CollisionSystem::AddSprite(SpriteObject* _sprite)
{
	if (std::find(sprites.begin(), sprites.end(), _sprite) == sprites.end())
	{
		sprites.emplace_back(_sprite);
	}
}



void CollisionSystem::RemoveSprite(SpriteObject* _sprite)
{
	auto position = std::find(sprites.begin(), sprites.end(), _sprite);
	if (position != sprites.end())
	{
		sprites.erase(position);
	}
}

void CollisionSystem::AddBoxCollider(BoxCollider* _collider)
{
	if (std::find(box_colliders.begin(), box_colliders.end(), _collider) == box_colliders.end())
	{
		box_colliders.emplace_back(_collider);
	}
}


void SolveQuad(int id , std::vector<SpriteObject*> sprites)
{
	for (auto _sprite1 : sprites)
	{
		for (auto _sprite2 : sprites)
		{
			if (_sprite1 == _sprite2)
			{
				continue;
			}
			auto sprite1_model = _sprite1->modelMatrix;
			auto sprite2_model = _sprite2->modelMatrix;

			float sprite1_origin_x = sprite1_model[3].x;
			float sprite1_origin_y = sprite1_model[3].y;

			float sprite1_c1_x = sprite1_origin_x + ((_sprite1->GetWidth()) * std::cos(glm::radians(_sprite1->rot_angle)) -
				(0) * std::sin(glm::radians(_sprite1->rot_angle)));

			float sprite1_c1_y = sprite1_origin_y + ((_sprite1->GetWidth()) * std::sin(glm::radians(_sprite1->rot_angle)) +
				(0) * std::cos(glm::radians(_sprite1->rot_angle)));

			float sprite1_c2_x = sprite1_origin_x + ((_sprite1->GetWidth()) * std::cos(glm::radians(_sprite1->rot_angle)) -
				(_sprite1->GetHeight()) * std::sin(glm::radians(_sprite1->rot_angle)));

			float sprite1_c2_y = sprite1_origin_y + ((_sprite1->GetWidth()) * std::sin(glm::radians(_sprite1->rot_angle)) +
				_sprite1->GetHeight() * std::cos(glm::radians(_sprite1->rot_angle)));

			float sprite1_c3_x = sprite1_origin_x + ((0) * std::cos(glm::radians(_sprite1->rot_angle)) -
				(_sprite1->GetHeight()) * std::sin(glm::radians(_sprite1->rot_angle)));

			float sprite1_c3_y = sprite1_origin_y + ((0) * std::sin(glm::radians(_sprite1->rot_angle)) +
				(_sprite1->GetHeight()) * std::cos(glm::radians(_sprite1->rot_angle)));

			float sprite2_origin_x = sprite2_model[3].x;
			float sprite2_origin_y = sprite2_model[3].y;

			float sprite2_c1_x = sprite2_origin_x + ((_sprite2->GetWidth()) * std::cos(glm::radians(_sprite2->rot_angle)) -
				(0) * std::sin(glm::radians(_sprite2->rot_angle)));

			float sprite2_c1_y = sprite2_origin_y + ((_sprite2->GetWidth()) * std::sin(glm::radians(_sprite2->rot_angle)) +
				(0) * std::cos(glm::radians(_sprite2->rot_angle)));

			float sprite2_c2_x = sprite2_origin_x + ((_sprite2->GetWidth()) * std::cos(glm::radians(_sprite2->rot_angle)) -
				(_sprite2->GetHeight()) * std::sin(glm::radians(_sprite2->rot_angle)));

			float sprite2_c2_y = sprite2_origin_y + ((_sprite2->GetWidth()) * std::sin(glm::radians(_sprite2->rot_angle)) +
				_sprite2->GetHeight() * std::cos(glm::radians(_sprite2->rot_angle)));

			float sprite2_c3_x = sprite2_origin_x + ((0) * std::cos(glm::radians(_sprite2->rot_angle)) -
				(_sprite2->GetHeight()) * std::sin(glm::radians(_sprite2->rot_angle)));

			float sprite2_c3_y = sprite2_origin_y + ((0) * std::sin(glm::radians(_sprite2->rot_angle)) +
				(_sprite2->GetHeight()) * std::cos(glm::radians(_sprite2->rot_angle)));




			float sprite1_x = sprite1_origin_x;
			if (sprite1_c1_x < sprite1_x)
			{
				sprite1_x = sprite1_c1_x;
			}
			if (sprite1_c2_x < sprite1_x)
			{
				sprite1_x = sprite1_c2_x;
			}
			if (sprite1_c3_x < sprite1_x)
			{
				sprite1_x = sprite1_c3_x;
			}

			float sprite1_x_max = sprite1_origin_x;
			if (sprite1_c1_x > sprite1_x_max)
			{
				sprite1_x_max = sprite1_c1_x;
			}
			if (sprite1_c2_x > sprite1_x_max)
			{
				sprite1_x_max = sprite1_c2_x;
			}
			if (sprite1_c3_x > sprite1_x_max)
			{
				sprite1_x_max = sprite1_c3_x;
			}

			float sprite1_y = sprite1_origin_y;
			if (sprite1_c1_y < sprite1_y)
			{
				sprite1_y = sprite1_c1_y;
			}
			if (sprite1_c2_y < sprite1_y)
			{
				sprite1_y = sprite1_c2_y;
			}
			if (sprite1_c3_y < sprite1_y)
			{
				sprite1_y = sprite1_c3_y;
			}

			float sprite1_y_max = sprite1_origin_y;
			if (sprite1_c1_y > sprite1_y_max)
			{
				sprite1_y_max = sprite1_c1_y;
			}
			if (sprite1_c2_y > sprite1_y_max)
			{
				sprite1_y_max = sprite1_c2_y;
			}
			if (sprite1_c3_y > sprite1_y_max)
			{
				sprite1_y_max = sprite1_c3_y;
			}

			float sprite2_x = sprite2_origin_x;
			if (sprite1_c2_x < sprite2_x)
			{
				sprite2_x = sprite2_c1_x;
			}
			if (sprite2_c2_x < sprite2_x)
			{
				sprite2_x = sprite2_c2_x;
			}
			if (sprite2_c3_x < sprite2_x)
			{
				sprite2_x = sprite2_c3_x;
			}

			float sprite2_x_max = sprite2_origin_x;
			if (sprite1_c2_x > sprite2_x_max)
			{
				sprite2_x_max = sprite2_c1_x;
			}
			if (sprite2_c2_x > sprite2_x_max)
			{
				sprite2_x_max = sprite2_c2_x;
			}
			if (sprite2_c3_x > sprite2_x_max)
			{
				sprite2_x_max = sprite2_c3_x;
			}

			float sprite2_y = sprite2_origin_y;
			if (sprite1_c2_y < sprite2_y)
			{
				sprite2_y = sprite2_c1_y;
			}
			if (sprite2_c2_y < sprite2_y)
			{
				sprite2_y = sprite2_c2_y;
			}
			if (sprite2_c3_y < sprite2_y)
			{
				sprite2_y = sprite2_c3_y;
			}

			float sprite2_y_max = sprite2_origin_y;
			if (sprite1_c2_y > sprite2_y_max)
			{
				sprite2_y_max = sprite2_c1_y;
			}
			if (sprite2_c2_y > sprite2_y_max)
			{
				sprite2_y_max = sprite2_c2_y;
			}
			if (sprite2_c3_y > sprite2_y_max)
			{
				sprite2_y_max = sprite2_c3_y;
			}




			if ((sprite1_x >= sprite2_x && sprite1_x <= sprite2_x_max) ||
				(sprite1_x_max >= sprite2_x && sprite1_x_max <= sprite2_x_max))
			{
				// objects overlap on the x axis
				if ((sprite1_y >= sprite2_y && sprite1_y <= sprite2_y_max) ||
					(sprite1_y_max >= sprite2_y && sprite1_y_max <= sprite2_y_max))
				{
					//objects also overlap on the y axis


					// displace objects by how much they overlap
					SpriteObject* sprite;
					if (_sprite1->moveable)
					{
						sprite = _sprite1;
					}
					else
					{
						sprite = _sprite2;
					}
					if (sprite->vector_x > 0)
					{
						//moving right
						float overlap_x = std::abs(sprite1_x_max - sprite2_x) + 1;
						if (_sprite2->moveable)
						{
							_sprite2->updatePos(overlap_x, 0, 0);
						}
						if (_sprite1->moveable)
						{
							_sprite1->updatePos(-overlap_x, 0, 0);
						}
					}
					else if (sprite->vector_x < 0)
					{
						//moving left
						float overlap_x = std::abs(sprite1_x - sprite2_x_max) + 1;
						if (_sprite2->moveable)
						{
							_sprite2->updatePos(-overlap_x, 0, 0);
						}
						if (_sprite1->moveable)
						{
							_sprite1->updatePos(overlap_x, 0, 0);
						}
					}
					if (sprite->vector_y > 0)
					{
						//moving up
						float overlap_y = std::abs(sprite1_y_max - sprite2_y) + 1;
						if (_sprite2->moveable)
						{
							_sprite2->updatePos(0, overlap_y, 0);
						}
						if (_sprite1->moveable)
						{
							_sprite1->updatePos(0, -overlap_y, 0);
						}
					}
					else if (sprite->vector_y < 0)
					{
						//moving down
						float overlap_y = std::abs(sprite1_y - sprite2_y_max) + 1;
						if (_sprite2->moveable)
						{
							_sprite2->updatePos(0, -overlap_y, 0);
						}
						if (_sprite1->moveable)
						{
							_sprite1->updatePos(0, overlap_y, 0);
						}
					}

					std::cout << "objects overlap \n";



				}
			}
		}
	}
}

void SolveQuadBox(int id, std::vector<BoxCollider> colliders)
{
	for (auto _collider1 : colliders)
	{
		for (auto _collider2 : colliders)
		{
			if (_collider1 == _collider2)
			{
				continue;
			}

			auto collider1_corners = _collider1.GetCorners();

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
			if (collider1_corners[2].y < sprite1_y)
			{
				sprite1_y = collider1_corners[2].y;
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

			auto collider2_corners = _collider2.GetCorners();

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
		pool.resize(8);
	}

	if (use_sprites)
	{

		quad1.clear();
		quad2.clear();
		quad3.clear();
		quad4.clear();

		for (auto sprite : sprites)
		{
			switch (sprite->quad)
			{
			case BoxCollider::Quad::BOTTOM_LEFT:
			{
				quad1.emplace_back(sprite);
				break;
			}
			case BoxCollider::Quad::BOTTOM_RIGHT:
			{
				quad2.emplace_back(sprite);
				break;
			}
			case BoxCollider::Quad::TOP_LEFT:
			{
				quad3.emplace_back(sprite);
				break;
			}
			case BoxCollider::Quad::TOP_RIGHT:
			{
				quad4.emplace_back(sprite);
				break;
			}
			case BoxCollider::Quad::TOP_LEFT_RIGHT:
			{
				quad4.emplace_back(sprite);
				quad3.emplace_back(sprite);
				break;
			}
			case BoxCollider::Quad::BOTTOM_LEFT_RIGHT:
			{
				quad1.emplace_back(sprite);
				quad2.emplace_back(sprite);
				break;
			}
			case BoxCollider::Quad::LEFT_SIDE:
			{
				quad3.emplace_back(sprite);
				quad1.emplace_back(sprite);
				break;
			}
			case BoxCollider::Quad::RIGHT_SIDE:
			{
				quad4.emplace_back(sprite);
				quad2.emplace_back(sprite);
				break;
			}
			case BoxCollider::Quad::ALL:
			{
				quad4.emplace_back(sprite);
				quad3.emplace_back(sprite);
				quad2.emplace_back(sprite);
				quad1.emplace_back(sprite);
				break;

			}


			}
		}

		// uses multi threading

		if (use_threads)
		{

			pool.push(SolveQuad, quad1);
			pool.push(SolveQuad, quad2);
			pool.push(SolveQuad, quad3);
			pool.push(SolveQuad, quad4);
			
			if (wait_threads)
			{
				while (pool.n_idle() <= 0)
				{
					// wait for threads
				}
			}
		}
		else
		{


			SolveQuad(0, quad1);
			SolveQuad(0, quad2);
			SolveQuad(0, quad3);
			SolveQuad(0, quad4);

		}




		int total_checks = 0;



		if (debug)
		{
			std::cout << "total collision checks: " + std::to_string(total_checks) + "\n";
		}
	}

	else
	{
		UpdateBoxColliders();
	}

}

void CollisionSystem::UpdateBoxColliders()
{
	boxquad1.clear();
	boxquad2.clear();
	boxquad3.clear();
	boxquad4.clear();

	for (auto collider : box_colliders)
	{
		auto corners = collider->GetCorners();
		BoxCollider _collider = { corners[0],collider->GetRotation(),collider->GetScale(),collider->GetID(),collider->GetScreen() };
		switch (collider->quad)
		{
		case BoxCollider::Quad::BOTTOM_LEFT:
		{
			boxquad1.emplace_back(_collider);
			break;
		}
		case BoxCollider::Quad::BOTTOM_RIGHT:
		{
			boxquad2.emplace_back(_collider);
			break;
		}
		case BoxCollider::Quad::TOP_LEFT:
		{
			boxquad3.emplace_back(_collider);
			break;
		}
		case BoxCollider::Quad::TOP_RIGHT:
		{
			boxquad4.emplace_back(_collider);
			break;
		}
		case BoxCollider::Quad::TOP_LEFT_RIGHT:
		{
			boxquad4.emplace_back(_collider);
			boxquad3.emplace_back(_collider);
			break;
		}
		case BoxCollider::Quad::BOTTOM_LEFT_RIGHT:
		{
			boxquad1.emplace_back(_collider);
			boxquad2.emplace_back(_collider);
			break;
		}
		case BoxCollider::Quad::LEFT_SIDE:
		{
			boxquad3.emplace_back(_collider);
			boxquad1.emplace_back(_collider);
			break;
		}
		case BoxCollider::Quad::RIGHT_SIDE:
		{
			boxquad4.emplace_back(_collider);
			boxquad2.emplace_back(_collider);
			break;
		}
		case BoxCollider::Quad::ALL:
		{
			boxquad4.emplace_back(_collider);
			boxquad3.emplace_back(_collider);
			boxquad2.emplace_back(_collider);
			boxquad1.emplace_back(_collider);
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






