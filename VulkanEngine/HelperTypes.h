#pragma once
#include <stdint.h>

struct Vector3
{
	Vector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	float x;
	float y;
	float z;
};

struct Vector2
{
	Vector2 operator - (Vector2 _other)
	{
		return { x - _other.x, y - _other.y };
	}

	Vector2 operator + (Vector2 _other)
	{
		return { x + _other.x, y + _other.y };
	}

	Vector2 operator += (Vector2 _other)
	{
		x += _other.x;
		y += _other.y;
		return {x, y};
	}

	Vector2 operator -= (Vector2 _other)
	{
		x += _other.x;
		y += _other.y;
		return { x, y };
	}

	Vector2(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	Vector2()
	{
		x = 0;
		y = 0;
	}

	void Normalise()
	{
		float z = x + y;
		if (z == 0)
		{
			return;
		}
		x /= z;
		y /= z;
	}

	float x;
	float y;
};

struct UVector2
{
	UVector2 operator -(UVector2 _other)
	{
		return { x - _other.x, y - _other.y };
	}

	UVector2 operator +(UVector2 _other)
	{
		return { x + _other.x, y + _other.y };
	}


	UVector2(uint32_t _x, uint32_t _y)
	{
		x = _x;
		y = _y;
	}

	UVector2()
	{
		x = 0;
		y = 0;
	}

	uint32_t x;
	uint32_t y;
};

struct Vector4x2
{
	Vector4x2(Vector2 _x, Vector2 _y, Vector2 _r, Vector2 _f)
	{
		x = _x;
		y = _y;
		r = _r;
		f = _f;
	}

	Vector2 x;
	Vector2 y;
	Vector2 r;
	Vector2 f;
};

struct Vector2x2
{
	Vector2x2(Vector2 _x, Vector2 _y)
	{
		x = _x;
		y = _y;
	}

	Vector2 x;
	Vector2 y;
};
