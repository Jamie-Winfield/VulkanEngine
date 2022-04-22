#pragma once
#include "spriteObject.h"
struct CollisionEvent
{
	CollisionEvent(SpriteObject* _object1, SpriteObject* _object2)
	{
		object1 = _object1;
		object2 = _object2;
	}

	bool Contains(SpriteObject* _object)
	{
		if (_object == object1 || _object == object2)
		{
			return true;
		}
		return false;
	}

	bool ContainsBoth(SpriteObject* _object1, SpriteObject* _object2)
	{
		if (_object1 == object1 || _object1 == object2)
		{
			if (_object2 == object1 || _object2 == object2)
			{
				return true;
			}
		}
		return false;
	}

	bool operator == (CollisionEvent _event)
	{
		if (this->ContainsBoth(_event.object1, _event.object2))
		{
			return true;
		}
		return false;
	}

	bool operator == (SpriteObject* _object)
	{
		if (this->Contains(_object))
		{
			return true;
		}
		return false;
	}


	SpriteObject* object1;
	SpriteObject* object2;
};
