#include "GameHelper.h"

using namespace GameHelper;

void GameHelper::Init(Engine* _engine)
{
	engine = _engine;
	font_loader = std::make_unique<LoadFont>(engine);
}

LoadFont* GameHelper::GetFontLoader()
{
	return font_loader.get();
}

TextureAtlas* GameHelper::CreateTextureAtlas(uint32_t width, uint32_t height, std::string _atlas_name)
{
	if (std::find(atlasNames.begin(), atlasNames.end(), _atlas_name) != atlasNames.end())
	{
		throw std::exception("Atlas name already in use!");
	}
	auto _atlas = std::make_unique<TextureAtlas>(width, height, engine, _atlas_name.c_str());
	atlasNames.emplace_back(_atlas_name);
	atlas_list.emplace_back(std::move(_atlas));
	return atlas_list.back().get();
}

bool GameHelper::CheckAtlasName(const char* _name)
{
	if (std::find(atlasNames.begin(), atlasNames.end(), _name) == atlasNames.end())
	{
		return true;
	}
	return false;
}

void GameHelper::ToggleFrameCounter(bool _enable)
{
	engine->SetFrameCounterEnable(_enable);
}

Text* GameHelper::CreateTextObject(std::string _text, Vector3 _color, uint32_t _font)
{
	std::unique_ptr<Text> text;

	text = std::make_unique<Text>(engine, _text, _color, _font);


	const std::vector<Vertex> vertices =
	{
		{{0.f, 1.f}, {1.0f, 0.0f, 0.0f, 1.f}, {1.f, 0.f}},
		{{1.f, 1.f}, {0.0f, 1.0f, 0.0f, 1.f}, {0.f, 0.f}},
		{{1.f, 0.f}, {0.0f, 0.0f, 1.0f, 1.f}, {0.f, 1.f}},
		{{0.f, 0.f}, {1.0f, 1.0f, 1.0f, 1.f}, {1.f, 1.f}}
	};

	const std::vector<uint16_t> indices = {
	0, 3, 2, 2, 1, 0
	};

	for (auto& vertex : vertices)
	{
		text->AddVertex(vertex);
	}
	for (auto& index : indices)
	{
		text->AddIndex(index);
	}

	text->CreateVertexBuffer(engine->getDevice(), engine->getPhysicalDevice(), engine->getRenderer()->getCommandPool(), engine->getGraphicsQueue());
	text->CreateIndexBuffer(engine->getDevice(), engine->getPhysicalDevice(), engine->getRenderer()->getCommandPool(), engine->getGraphicsQueue());

	bool genString = true;

	std::string random_string;

	while (genString)
	{
		const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

		std::random_device random_device;
		std::mt19937 generator(random_device());
		std::uniform_int_distribution<> distribution(0, static_cast<int>(CHARACTERS.size()) - 1);

		random_string.clear();

		for (std::size_t i = 0; i < 10; ++i)
		{
			random_string += CHARACTERS[distribution(generator)];
		}
		if (CheckAtlasName(random_string.c_str()))
		{
			atlasNames.emplace_back(random_string);
			genString = false;
			break;
		}
	}

	auto atlas = font_loader->LoadText(_text, _font, random_string.c_str(), _color);

	text->SetAtlas(atlas.get(), random_string);

	auto texture = engine->getRenderer()->createTextureImage(engine->getDevice(), engine->getPhysicalDevice(),
		text->GetAtlasName().c_str(), engine->getGraphicsQueue());
	text->SetTextureImage(texture.first);
	text->SetScale(texture.second.x, texture.second.y);
	text->SetTextureImageView(engine->getRenderer()->createImageView(text->GetTextureImage(), VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_ASPECT_COLOR_BIT, engine->getDevice()));

	text->FlipRenderable();

	atlas_list.emplace_back(std::move(atlas));
	auto text_return = engine->AddCreatedSprite(std::move(text));

	return dynamic_cast<Text*>(text_return);
}

SpriteObject* GameHelper::CreateSprite(const char* _filename)
{
	return engine->createSprite(_filename);
}

void GameHelper::ChangeText(Text* _textobject, std::string _text, Vector3 _color, uint32_t _font)
{
	engine->getRenderer()->ResetVKImage(_textobject->GetAtlas()->image, _textobject->GetAtlas()->imageView, engine->getDevice());
	
	auto atlas = font_loader->LoadText(_text, _font, _textobject->GetAtlasName().c_str(), _color);

	_textobject->SetAtlas(atlas.get(), _textobject->GetAtlasName());

	auto texture = engine->getRenderer()->createTextureImage(engine->getDevice(), engine->getPhysicalDevice(),
		_textobject->GetAtlasName().c_str(), engine->getGraphicsQueue());
	_textobject->SetTextureImage(texture.first);
	_textobject->SetScale(texture.second.x, texture.second.y);
	_textobject->SetTextureImageView(engine->getRenderer()->createImageView(_textobject->GetTextureImage(), VK_FORMAT_R8G8B8A8_SRGB,
		VK_IMAGE_ASPECT_COLOR_BIT, engine->getDevice()));
	atlas_list.emplace_back(std::move(atlas));
	engine->getRenderer()->ResetCommandBuffers(engine->getDevice(),engine->getWindow(),engine->getPhysicalDevice(), engine->getSurface());
	
}

void GameHelper::EnableCollisionSystem(bool _enable)
{
	if (_enable)
	{
		engine->EnableCollisionSystem();
	}
	else
	{
		engine->DisableCollisionSystem();
	}
}

CollisionSystem* GameHelper::GetCollisionSystem()
{
	return engine->GetCollisionSystem();
}

void GameHelper::StopGame()
{
	engine->StopGame();
}

Renderer* GameHelper::GetRenderer()
{
	return engine->getRenderer();
}

void GameHelper::SetGameTime(float _time)
{
	gametime = _time;
}

float GameHelper::GameTimeDelta()
{
	return gametime;
}
