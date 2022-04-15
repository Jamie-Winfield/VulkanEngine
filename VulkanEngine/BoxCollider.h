#pragma once
#include "HelperTypes.h"

#include <array>

class SpriteObject;

class BoxCollider
{
public:

	bool operator== (BoxCollider& _collider)
	{
		return _collider.GetID() == ID;
	}


	enum Quad
	{
		TOP_LEFT = 4,
		TOP_RIGHT = 5,
		TOP_LEFT_RIGHT = 20,
		BOTTOM_LEFT = 2,
		BOTTOM_RIGHT = 3,
		BOTTOM_LEFT_RIGHT = 6,
		LEFT_SIDE = 8,
		RIGHT_SIDE = 15,
		ALL = 120
	}quad = ALL;


	BoxCollider(Vector2 _position, float _rotation, Vector2 _scale, uint16_t _id, Vector2 _screen, SpriteObject* _parent)
	{
		origin = _position;
		scale = _scale;
		rotation = _rotation;
		ID = _id;
		screen = _screen;
		parent = _parent;
		prev_origin = origin;
		prev_corner1 = corner1;
		prev_corner2 = corner2;
		prev_corner3 = corner3;

		SetCorners();
	}

	void SetPosition(Vector2 _position);

	void SetRotation(float _rotation);

	void UpdateBox(Vector2 _position, float _rotation, Vector2 _scale);

	SpriteObject* GetParent()
	{
		return parent;
	}

	std::array<Vector2, 4> GetCorners();

	float GetRotation()
	{
		return rotation;
	}

	Vector2 GetScale()
	{
		return scale;
	}

	Vector2 GetScreen()
	{
		return screen;
	}

	Vector2 GetVector()
	{
		return vector;
	}

	void SetMoveable(bool _moveable)
	{
		moveable = _moveable;
	}

	bool GetMoveable()
	{
		return moveable;
	}

	

	uint16_t GetID();

private:
	Vector2 origin =	{ 0, 0 };
	Vector2 corner1 =	{ 0, 0 };
	Vector2 corner2 =	{ 0, 0 };
	Vector2 corner3 =	{ 0, 0 };
	Vector2 scale =		{ 0, 0 };
	Vector2 screen =	{ 0, 0 };
	Vector2 vector =	{ 0, 0 };

	Vector2 prev_origin = { 0, 0 };
	Vector2 prev_corner1 = { 0, 0 };
	Vector2 prev_corner2 = { 0, 0 };
	Vector2 prev_corner3 = { 0, 0 };

	bool moveable = true;



	uint16_t ID;

	float rotation = 0;

	void SetCorners();
	void SetQuad();
	void UpdateVector();

	SpriteObject* parent;


};

