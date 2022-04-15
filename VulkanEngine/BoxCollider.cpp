#include "BoxCollider.h"
#include <cmath>
#include <glm/glm.hpp>

void BoxCollider::SetPosition(Vector2 _position)
{
	origin = _position;
	SetCorners();
}

void BoxCollider::SetRotation(float _rotation)
{
	rotation = _rotation;
	SetCorners();
}

void BoxCollider::UpdateBox(Vector2 _position, float _rotation, Vector2 _scale)
{
    origin = _position;
    rotation = _rotation;
    scale = _scale;
    

    SetCorners();
}

std::array<Vector2, 4> BoxCollider::GetCorners()
{
	std::array<Vector2, 4> corners = 
	{	
		origin,
		corner1,
		corner2,
		corner3
	};

	return corners;
}

uint16_t BoxCollider::GetID()
{
	return ID;
}

void BoxCollider::SetCorners()
{

	auto rotation_radians = glm::radians(rotation);

	corner1.x = origin.x + (scale.x * std::cos(rotation_radians));
	corner1.y = origin.y + (scale.x * std::sin(rotation_radians));

	corner2.x = origin.x + ((scale.x * std::cos(rotation_radians)) - (scale.y * std::sin(rotation_radians)));
	corner2.y = origin.y + ((scale.x * std::sin(rotation_radians) + scale.y * std::cos(rotation_radians)));

	corner3.x = origin.x + (-(scale.y * std::sin(rotation_radians)));
	corner3.y = origin.y + (scale.y * std::cos(rotation_radians));

    SetQuad();
    UpdateVector();
}

void BoxCollider::SetQuad()
{

    // bottom left = 2
    // bottom right = 3
    // top left = 4
    // top right = 5

    int quad_int = 0;

    // four corners of cube
    int p_1 = 0;
    int p_2 = 0;
    int p_3 = 0;
    int p_4 = 0;

    // bottom left corner
    if (origin.x >= screen.x / 2)
    {
        if (origin.y >= screen.y / 2)
        {
            p_1 = 5;
        }
        else
        {
            p_1 = 3;
        }
    }
    else if (origin.y >= screen.y / 2)
    {
        p_1 = 4;
    }
    else
    {
        p_1 = 2;
    }

    // top left corner
    if (origin.x >= screen.x / 2)
    {
        if (origin.y + scale.y >= screen.y / 2)
        {
            p_3 = 5;
        }
        else
        {
            p_3 = 3;
        }
    }
    else if (origin.y + scale.y >= screen.y / 2)
    {
        p_3 = 4;
    }
    else
    {
        p_3 = 2;
    }

    // top right corner
    if (origin.x + scale.x >= screen.x / 2)
    {
        if (origin.y + scale.y >= screen.y / 2)
        {
            p_4 = 5;
        }
        else
        {
            p_4 = 3;
        }
    }
    else if (origin.y + scale.y >= screen.y / 2)
    {
        p_4 = 4;
    }
    else
    {
        p_4 = 2;
    }

    // bottom right corner
    if (origin.x + scale.x >= screen.x / 2)
    {
        if (origin.y >= screen.y / 2)
        {
            p_2 = 5;
        }
        else
        {
            p_2 = 3;
        }
    }
    else if (origin.y >= screen.y / 2)
    {
        p_2 = 4;
    }
    else
    {
        p_2 = 2;
    }

    quad_int = p_1;
    if (p_2 != p_1)
    {
        quad_int *= p_2;
    }
    if (p_3 != p_2 && p_3 != p_1)
    {
        quad_int *= p_3;
    }
    if (p_4 != p_3 && p_4 != p_2 && p_4 != p_1)
    {
        quad_int *= p_4;
    }

    quad = static_cast<BoxCollider::Quad>(quad_int);
}

void BoxCollider::UpdateVector()
{
    Vector2 vector0 = origin - prev_origin;
    Vector2 vector1 = corner1 - prev_corner1;
    Vector2 vector2 = corner2 - prev_corner2;
    Vector2 vector3 = corner3 - prev_corner3;

    vector = vector0 + vector1 + vector2 + vector3;

    prev_origin = origin;
    prev_corner1 = corner1;
    prev_corner2 = corner2;
    prev_corner3 = corner3;

}
