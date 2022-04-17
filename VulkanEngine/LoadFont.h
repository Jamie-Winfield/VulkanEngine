#pragma once




#include "Engine.h"
#include "TextureAtlas.h"

#include <ft2build.h>
#include <freetype/freetype.h>
class LoadFont
{
private:

	bool Init();


	FT_Library lib;

	FT_Face face;

	Engine* engine;

	std::unique_ptr<TextureAtlas> atlas;


public:
	LoadFont(Engine* _engine);

	void AddFont();
};

