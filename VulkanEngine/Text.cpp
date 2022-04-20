#include "Text.h"
#include <random>

Text::Text(Engine* engine, std::string _text, Vector3 _color, uint32_t _font_id) :
    Renderable(engine->getDevice(), engine->getRenderer()->GetCommandPool(), engine->getGraphicsQueue(),
        engine->getPhysicalDevice(), engine->GetScreenSize())
{
    text = _text;

}


std::string Text::GetText()
{
    return text;
}

std::string Text::GetAtlasName()
{
    return atlas_name;
}

void Text::SetAtlas(TextureAtlas* _atlas, std::string _atlas_name)
{
    atlas = _atlas;
    atlas_name = _atlas_name;
}

void Text::Update()
{
}
