#pragma once
#include "Renderable.h"
#include "LoadFont.h"
#include "TextureAtlas.h"
#include "Engine.h"
class Text : public Renderable
{
public:
	Text(Engine* engine, std::string _text, Vector3 _color, uint32_t _font_id);


	std::string GetText();
	std::string GetAtlasName();
	TextureAtlas* GetAtlas();
	void SetAtlas(TextureAtlas* _atlas, std::string _atlas_name);
	void Update() override;



private:




public:




private:
	std::string text;
	std::string atlas_name;
	TextureAtlas* atlas;
};

