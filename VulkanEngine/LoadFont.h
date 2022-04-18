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


	std::vector<FT_Face> faces;
	std::vector<uint32_t> face_sizes;

	Engine* engine;

	std::unique_ptr<TextureAtlas> atlas;


public:
	LoadFont(Engine* _engine);
	uint32_t CreateFont(const char* fontname, uint32_t pixel_size );
	std::unique_ptr<TextureAtlas> LoadText(std::string text, uint32_t font_id, const char* text_name, Vector3 color);

};

