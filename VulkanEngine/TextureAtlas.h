#pragma once
#include "HelperTypes.h"
#include "Engine.h"



struct Area
{
	Area(Vector2 _position, UVector2 _size)
	{
		position = _position;
		size = _size;
		posMax = Vector2(position.x + size.x, position.y + size.y);
	}

	Area()
	{
		position = Vector2(0, 0);
		size = UVector2(0, 0);
		posMax = Vector2(position.x + size.x, position.y + size.y);
	}

	Vector2 position;
	Vector2 posMax;
	UVector2 size;
};

class TextureAtlas
{
public:
	TextureAtlas(uint32_t width, uint32_t height, Engine* _engine, const char* _atlasname);

	bool AddImage(const char* filename);

	Vector2x2 GetUV(const char* filename);

private:
	void CreateAtlas();


	Engine* engine;
	UVector2 size;
	VkImage image;

	VkImageView imageView;

	std::vector<std::pair<const char*,Area>> usedAreas;

	const char* atlasname;

	Area lastArea;
	Area rowStartArea;

};


