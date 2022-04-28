#pragma once
#include "Engine.h"
#include "LoadFont.h"
#include "TextureAtlas.h"
#include "Text.h"
#include <random>

namespace GameHelper
{
	namespace
	{
		std::unique_ptr<LoadFont> font_loader;
		Engine* engine;

		std::vector < std::unique_ptr<TextureAtlas>> atlas_list;
		std::vector<std::string> atlasNames;
	}

	void Init(Engine* _engine);

	LoadFont* GetFontLoader();

	TextureAtlas* CreateTextureAtlas(uint32_t width, uint32_t height, std::string _atlas_name);

	/// <summary>
	/// Returns true if name is useable
	/// </summary>
	bool CheckAtlasName(const char* _name);

	void ToggleFrameCounter(bool _enable);

	Text* CreateTextObject(std::string _text, Vector3 _color, uint32_t _font);

	SpriteObject* CreateSprite(const char* _filename);

	void ChangeText(Text* _textobject, std::string _text, Vector3 _color, uint32_t _font);

	void EnableCollisionSystem(bool _enable);

	CollisionSystem* GetCollisionSystem();

	void StopGame();

	Renderer* GetRenderer();
}
