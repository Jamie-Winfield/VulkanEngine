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
	return static_cast<uint32_t>(faces.size() - 1);
}

std::unique_ptr<TextureAtlas> LoadFont::LoadText(std::string text, uint32_t font_id, const char* text_name, Vector3 color)
{
	if (font_id >= faces.size())
	{
		throw std::exception("font_id too large for faces vector!");
	}
	FT_Face _face = faces[font_id];

	

	auto size = face_sizes[font_id] + 1;

	unsigned int width = 0;
	unsigned int height = size;
	unsigned int rows = 0;
	unsigned int current_width = 0;
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
				current_width += size / 2;
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



	auto atlas = std::make_unique<TextureAtlas>(static_cast<uint32_t>(width + 1), static_cast<uint32_t>(((height + size / 2) * (rows + 1)) + 1),
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
				atlas->AddEmptySpace(size / 2, size);
			}
			else
			{
				next_char_new_line = false;
				atlas->AddEmptySpaceNewLine(size / 2, size);
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

		for (unsigned int y = 0; y < height; ++y)
		{
			src = start;
			for (unsigned int x = 0; x < width; ++x)
			{
				auto value = *src;
				src++;
				buffer[dst++] = static_cast<uint8_t>(color.x);
				buffer[dst++] = static_cast<uint8_t>(color.y);
				buffer[dst++] = static_cast<uint8_t>(color.z);
				buffer[dst++] = value;
			}
			start += _face->glyph->bitmap.pitch;
		}
		float offset = static_cast<float>(size - _face->glyph->metrics.horiBearingY / static_cast<float>(64));
		

		if (next_char_new_line)
		{
			
			atlas->AddTextNewLine(name, buffer.data(), width, height, static_cast<int>(offset));
			next_char_new_line = false;
		}
		else
		{
			atlas->AddText(name, buffer.data(), width, height, static_cast<int>(offset));
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

