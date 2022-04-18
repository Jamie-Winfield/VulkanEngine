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

uint32_t LoadFont::CreateFont(const char* fontname, uint32_t pixel_size)
{
	FT_Face _face;

	auto error = FT_New_Face(lib, fontname, 0, &_face);
	if (error)
	{
		throw std::exception("failed to load font file!");
	}

	error = FT_Set_Pixel_Sizes(_face, 0, pixel_size);
	if (error)
	{
		throw std::exception("failed to set pixel sizes!");
	}

	int total_glyphs = _face->num_glyphs;

	if (total_glyphs <= 0)
	{
		throw std::exception("no glyphs in font found!");
	}

	faces.emplace_back(std::move(_face));
	face_sizes.emplace_back(pixel_size);
	return faces.size() - 1;
}

std::unique_ptr<TextureAtlas> LoadFont::LoadText(std::string text, uint32_t font_id, const char* text_name)
{
	if (font_id >= faces.size())
	{
		throw std::exception("font_id too large for faces vector!");
	}
	FT_Face _face = faces[font_id];

	

	auto size = face_sizes[font_id] + 1;

	int width = 0;
	int height = size;
	int rows = 0;
	int current_width = 0;
	for (auto _char : text)
	{
		
		if (_char != 10)
		{
			if (_char != 32)
			{
				auto glyph_index = FT_Get_Char_Index(_face, _char);
				FT_Load_Glyph(_face, glyph_index, FT_LOAD_DEFAULT);
				FT_Render_Glyph(_face->glyph, FT_RENDER_MODE_NORMAL);
				current_width += _face->glyph->bitmap.width;
			}
			else
			{
				current_width += size;
			}
			if (_face->glyph->bitmap.rows > height)
			{
				height = _face->glyph->bitmap.rows;
			}
		}
		else
		{
			rows++;
			if (current_width > width)
			{
				width = current_width;
			}
			current_width = 0;
		}

	}
	if (current_width > width)
	{
		width = current_width;
	}



	auto atlas = std::make_unique<TextureAtlas>(static_cast<uint32_t>(width + 1), static_cast<uint32_t>((height * (rows + 1)) + 1),
		engine, text_name);
	bool next_char_new_line = false;
	for (auto _char : text)
	{
		if(_char == 10)
		{
			next_char_new_line = true;
			continue;
		}
		else if(_char == 32)
		{
			if (!next_char_new_line)
			{
				atlas->AddEmptySpace(size, size);
			}
			else
			{
				next_char_new_line = false;
				atlas->AddEmptySpaceNewLine(size, size);
			}
			continue;
		}
		
		auto glyph_index = FT_Get_Char_Index(_face, _char);
		auto error = FT_Load_Glyph(_face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{
			throw std::exception("failed to load glyph!");
		}
		error = FT_Render_Glyph(_face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
		{
			throw std::exception("failed to render glyph!");
		}

		if (!_face->glyph->bitmap.buffer)
		{
			continue;
		}

		const char* name = &_char;

		auto width = _face->glyph->bitmap.width;
		auto height = _face->glyph->bitmap.rows;
		auto bufferSize = width * height * 4;

		std::vector<uint8_t> buffer(bufferSize);

		uint8_t* src = _face->glyph->bitmap.buffer;
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
			start += _face->glyph->bitmap.pitch;
		}

		if (next_char_new_line)
		{
			atlas->AddTextNewLine(name, buffer.data(), width, height, size);
			next_char_new_line = false;
		}
		else
		{
			atlas->AddText(name, buffer.data(), width, height, size);
		}
	}

	return std::move(atlas);
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
	auto error = FT_New_Face(lib, "Fonts/SalmaproMedium-0Wooo.ttf", 0, &face);
	if (error)
	{
		throw std::exception("failed to load font file!");
	}

	error = FT_Set_Pixel_Sizes(face, 0, 50);
	if (error)
	{
		throw std::exception("failed to set pixel sizes!");
	}

	


	auto glyph_index = FT_Get_Char_Index(face, 68);
	FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
	FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

	


	const char* name = "ID" + 0;

	auto width = face->glyph->bitmap.width;
	auto height = face->glyph->bitmap.rows;
	auto bufferSize = width * height * 4;

	int total_glyphs = face->num_glyphs;

	float x = width;
	if (height > width)
	{
		x = height;
	}



	float atlas_size = 51 * (std::roundf(std::sqrtf(total_glyphs) + 1));

	atlas = std::make_unique<TextureAtlas>(static_cast<uint32_t>(atlas_size), static_cast<uint32_t>(atlas_size),
		engine, "atlas2");

	
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


		atlas->AddText(name, buffer.data(), width, height, 51);
	}
	
	
}
