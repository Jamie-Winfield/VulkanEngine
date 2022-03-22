#include "CollsionSystem.h"

void CollsionSystem::AddSprite(SpriteObject* _sprite)
{
	sprites.emplace_back(_sprite);
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



	int total_checks = 0;


	// check bottom right quad
	for (auto _sprite1 : quad1)
	{
		for (auto _sprite2 : quad1)
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
			total_checks++;
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

	// check bottom left quad
	for (auto _sprite1 : quad2)
	{
		for (auto _sprite2 : quad2)
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
			total_checks++;
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

	// check top left quad
	for (auto _sprite1 : quad3)
	{
		for (auto _sprite2 : quad3)
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
			total_checks++;
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

	// check top right quad
	for (auto _sprite1 : quad4)
	{
		for (auto _sprite2 : quad4)
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
			total_checks++;
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

	if (debug)
	{
		std::cout << "total collision checks: " + std::to_string(total_checks) + "\n";
	}
	
}


