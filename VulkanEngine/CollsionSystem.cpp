#include "CollsionSystem.h"
#include <thread>

void CollsionSystem::AddSprite(SpriteObject* _sprite)
{
	sprites.emplace_back(_sprite);
}

void SolveQuad1(std::vector<SpriteObject*> sprites)
{
	// check bottom left quad
	for (auto _sprite1 : sprites)
	{
		for (auto _sprite2 : sprites)
		{
			if (_sprite1 == _sprite2)
			{
				continue;
			}
			float sprite1_x = _sprite1->getXPos();
			float sprite1_y = _sprite1->getYPos();
			float sprite1_x_max = _sprite1->getXPos() + _sprite1->GetWidth();
			float sprite1_y_max = _sprite1->getYPos() + _sprite1->GetHeight();

			float sprite2_x = _sprite2->getXPos();
			float sprite2_y = _sprite2->getYPos();
			float sprite2_x_max = _sprite2->getXPos() + _sprite2->GetWidth();
			float sprite2_y_max = _sprite2->getYPos() + _sprite2->GetHeight();
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

void SolveQuad2(std::vector<SpriteObject*> sprites)
{
	// check bottom right quad
	for (auto _sprite1 : sprites)
	{
		for (auto _sprite2 : sprites)
		{
			if (_sprite1 == _sprite2)
			{
				continue;
			}
			float sprite1_x = _sprite1->getXPos();
			float sprite1_y = _sprite1->getYPos();
			float sprite1_x_max = _sprite1->getXPos() + _sprite1->GetWidth();
			float sprite1_y_max = _sprite1->getYPos() + _sprite1->GetHeight();

			float sprite2_x = _sprite2->getXPos();
			float sprite2_y = _sprite2->getYPos();
			float sprite2_x_max = _sprite2->getXPos() + _sprite2->GetWidth();
			float sprite2_y_max = _sprite2->getYPos() + _sprite2->GetHeight();
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

void SolveQuad3(std::vector<SpriteObject*> sprites)
{
	// check top left quad
	for (auto _sprite1 : sprites)
	{
		for (auto _sprite2 : sprites)
		{
			if (_sprite1 == _sprite2)
			{
				continue;
			}
			float sprite1_x = _sprite1->getXPos();
			float sprite1_y = _sprite1->getYPos();
			float sprite1_x_max = _sprite1->getXPos() + _sprite1->GetWidth();
			float sprite1_y_max = _sprite1->getYPos() + _sprite1->GetHeight();

			float sprite2_x = _sprite2->getXPos();
			float sprite2_y = _sprite2->getYPos();
			float sprite2_x_max = _sprite2->getXPos() + _sprite2->GetWidth();
			float sprite2_y_max = _sprite2->getYPos() + _sprite2->GetHeight();
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

void SolveQuad4(std::vector<SpriteObject*> sprites)
{
	// check top right quad
	for (auto _sprite1 : sprites)
	{
		for (auto _sprite2 : sprites)
		{
			if (_sprite1 == _sprite2)
			{
				continue;
			}
			float sprite1_x = _sprite1->getXPos();
			float sprite1_y = _sprite1->getYPos();
			float sprite1_x_max = _sprite1->getXPos() + _sprite1->GetWidth();
			float sprite1_y_max = _sprite1->getYPos() + _sprite1->GetHeight();

			float sprite2_x = _sprite2->getXPos();
			float sprite2_y = _sprite2->getYPos();
			float sprite2_x_max = _sprite2->getXPos() + _sprite2->GetWidth();
			float sprite2_y_max = _sprite2->getYPos() + _sprite2->GetHeight();
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

void SolveQuad(std::vector<SpriteObject*> sprites)
{
	// check top right quad
	for (auto _sprite1 : sprites)
	{
		for (auto _sprite2 : sprites)
		{
			if (_sprite1 == _sprite2)
			{
				continue;
			}
			float sprite1_x = _sprite1->getXPos();
			float sprite1_y = _sprite1->getYPos();
			float sprite1_x_max = _sprite1->getXPos() + _sprite1->GetWidth();
			float sprite1_y_max = _sprite1->getYPos() + _sprite1->GetHeight();

			float sprite2_x = _sprite2->getXPos();
			float sprite2_y = _sprite2->getYPos();
			float sprite2_x_max = _sprite2->getXPos() + _sprite2->GetWidth();
			float sprite2_y_max = _sprite2->getYPos() + _sprite2->GetHeight();
			if ((sprite1_x >= sprite2_x && sprite1_x <= sprite2_x_max) ||
				(sprite1_x_max >= sprite2_x && sprite1_x_max <= sprite2_x_max))
			{
				// objects overlap on the x axis
				if ((sprite1_y >= sprite2_y && sprite1_y <= sprite2_y_max) ||
					(sprite1_y_max >= sprite2_y && sprite1_y_max <= sprite2_y_max))
				{
					//objects also overlap on the y axis
					

					// displace objects by how much they overlap

					if (_sprite1->vector_x > 0 && _sprite1->moveable)
					{
						//moving right
						float overlap_x = std::abs(sprite1_x_max - sprite2_x) + 1;
						_sprite1->updatePos(-overlap_x, 0, 0);
					}
					else if (_sprite1->vector_x < 0 && _sprite1->moveable)
					{
						//moving left
						float overlap_x = std::abs(sprite1_x - sprite2_x_max) + 1;
						_sprite1->updatePos(overlap_x, 0, 0);
					}
					if (_sprite1->vector_y > 0 && _sprite1->moveable)
					{
						//moving up
						float overlap_y = std::abs(sprite1_y_max - sprite2_y) + 1;
						_sprite1->updatePos(0, -overlap_y, 0);
					}
					else if (_sprite1->vector_y < 0 && _sprite1->moveable)
					{
						//moving down
						float overlap_y = std::abs(sprite1_y - sprite2_y_max) + 1;
						_sprite1->updatePos(0, overlap_y, 0);
					}

					std::cout << "objects overlap \n";
					


				}
			}
		}
	}
}

void CollsionSystem::UpdateCollsions()
{
	quad1.clear();
	quad2.clear();
	quad3.clear();
	quad4.clear();
	
	for (auto sprite : sprites)
	{
		switch (sprite->quad)
		{
			case SpriteObject::Quad::BOTTOM_LEFT:
			{
				quad1.emplace_back(sprite);
				break;
			}
			case SpriteObject::Quad::BOTTOM_RIGHT:
			{
				quad2.emplace_back(sprite);
				break;
			}
			case SpriteObject::Quad::TOP_LEFT:
			{
				quad3.emplace_back(sprite);
				break;
			}
			case SpriteObject::Quad::TOP_RIGHT:
			{
				quad4.emplace_back(sprite);
				break;
			}
			case SpriteObject::Quad::TOP_LEFT_RIGHT:
			{
				quad4.emplace_back(sprite);
				quad3.emplace_back(sprite);
				break;
			}
			case SpriteObject::Quad::BOTTOM_LEFT_RIGHT:
			{
				quad1.emplace_back(sprite);
				quad2.emplace_back(sprite);
				break;
			}
			case SpriteObject::Quad::LEFT_SIDE:
			{
				quad3.emplace_back(sprite);
				quad1.emplace_back(sprite);
				break;
			}
			case SpriteObject::Quad::RIGHT_SIDE:
			{
				quad4.emplace_back(sprite);
				quad2.emplace_back(sprite);
				break;
			}
			case SpriteObject::Quad::ALL:
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

	/*
	std::thread t1(SolveQuad1,quad1);
	std::thread t2(SolveQuad2, quad2);
	std::thread t3(SolveQuad3, quad3);
	std::thread t4(SolveQuad4, quad4);

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	*/

	SolveQuad1(quad1);
	SolveQuad2(quad2);
	SolveQuad3(quad3);
	SolveQuad4(quad4);

	SolveQuad(quad1);
	SolveQuad(quad2);
	SolveQuad(quad3);
	SolveQuad(quad4);

	int total_checks = 0;



	if (debug)
	{
		std::cout << "total collision checks: " + std::to_string(total_checks) + "\n";
	}
	
}






