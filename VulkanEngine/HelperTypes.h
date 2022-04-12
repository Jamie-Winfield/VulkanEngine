#pragma once

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

	float x;
	float y;
};