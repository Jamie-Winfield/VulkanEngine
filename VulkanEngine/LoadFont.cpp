#include "LoadFont.h"
#include <exception>




LoadFont::LoadFont(Engine* _engine)
{
	engine = _engine;
	if (!Init())
	{
		throw std::exception("failed to initilise freetype library!");
	}
}



bool LoadFont::Init()
{
	auto error = FT_Init_FreeType(&lib);
	if (error)
	{
		return false;
	}
	return true;
}

void LoadFont::AddFont()
{
	auto error = FT_New_Face(lib, "Fonts/Nasa21-l23X.ttf", 0, &face);
	if (error)
	{
		throw std::exception("failed to load font file!");
	}

	error = FT_Set_Pixel_Sizes(face, 0, 24);
	if (error)
	{
		throw std::exception("failed to set pixel sizes!");
	}

	int total_glyphs = face->num_glyphs;

	float atlas_size = 60;

	atlas = std::make_unique<TextureAtlas>(static_cast<uint32_t>(atlas_size), static_cast<uint32_t>(atlas_size),
		engine, "atlas2");


	auto glyph_index = FT_Get_Char_Index(face, 65);
	FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
	FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

	const char* name = "ID" + 0;

	auto width = face->glyph->bitmap.width;
	auto height = face->glyph->bitmap.rows;
	auto bufferSize = width * height * 4;

	std::vector<uint8_t> buffer(bufferSize);

	uint8_t* src = face->glyph->bitmap.buffer;
	uint8_t* start = src;
	int dst = 0;

	for (int y = 0; y < height; ++y)
	{
		src = start;
		for (int x = 0; x < width; ++x)
		{
			auto value = *src;
			src++;
			buffer[dst++] = 0xff;
			buffer[dst++] = 0xff;
			buffer[dst++] = 0xff;
			buffer[dst++] = value;
		}
		start += face->glyph->bitmap.pitch;
	}

	atlas->AddImage(name, buffer.data(), width, height);
	atlas->AddImage(name, face->glyph->bitmap.buffer, width, height);

	/*
	for (int i = 0; i < total_glyphs; ++i)
	{
		error = FT_Load_Glyph(face, i, FT_LOAD_DEFAULT);
		if (error)
		{
			throw std::exception("failed to load glyph!");
		}
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
		{
			throw std::exception("failed to render glyph!");
		}

		if (!face->glyph->bitmap.buffer)
		{
			continue;
		}

		const char* name = "ID" + i;

		auto width = face->glyph->bitmap.width;
		auto height = face->glyph->bitmap.rows;
		auto bufferSize = width * height * 4;

		std::vector<uint8_t> buffer(bufferSize);

		uint8_t* src = face->glyph->bitmap.buffer;
		uint8_t* start = src;
		int dst = 0;

		for (int y = 0; y < height; ++y)
		{
			src = start;
			for (int x = 0; x < width; ++x)
			{
				auto value = *src;
				src++;
				buffer[dst++] = 0xff;
				buffer[dst++] = 0xff;
				buffer[dst++] = 0xff;
				buffer[dst++] = value;
			}
			start += face->glyph->bitmap.pitch;
		}


		atlas->AddImage(name, buffer.data(), 24, 24);
	}
	*/
	
}
