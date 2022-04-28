#include "Game.h"
#include "Colors.h"

void Game::Start()
{
	// Creating a sprite normally is as simple as providing a texture file
	sprite = GameHelper::CreateSprite("textures/texture.jpg");

	// the scale of a sprite is by default set to the size of the texture provided
	// however can be changed using SetScale
	sprite->SetScale(100, 100);

	// The sprite can then be positioned
	sprite->SetPos(200, 200);

	// The sprite can also have its z order set
	sprite->SetZOrder(1);

	// alternatively a texture can first be placed into a user defined texture atlas
	// an atlas just needs a size and a name to be created through the game helper
	atlas = GameHelper::CreateTextureAtlas(1000, 1000, "atlas1");

	// next textures can be added to the atlas
	atlas->AddImage("textures/texture.jpg");
	atlas->AddImage("textures/whitepix.png");
	atlas->AddImage("textures/NoTexture.png");
	// images are automatically placed in the atlas, if there is insufficient space
	// an error will be thrown

	// next the atlas can be assigned to a sprite the same way as normally creating a sprite
	atlas_sprite = GameHelper::CreateSprite("atlas1");

	// then the actual image from the atlas can be set on the sprite
	atlas_sprite->SetUVs(atlas->GetUV("textures/texture.jpg"));

	atlas_sprite->SetScale(100, 100);
	atlas_sprite->SetPos(400, 200);

	// Multiple sprites can use the same atlas

	atlas_sprite2 = GameHelper::CreateSprite("atlas1");
	atlas_sprite2->SetUVs(atlas->GetUV("textures/whitepix.png"));
	atlas_sprite2->SetScale(100, 100);
	atlas_sprite2->SetPos(600, 200);


	// To create text first a font needs to be loaded, the font must be of a ttf format
	auto font_loader = GameHelper::GetFontLoader();
	auto font_id = font_loader->CreateFont("fonts/SalmaproMedium-0Wooo.ttf", 25);

	// the font id returned is then used when creating text
	text = GameHelper::CreateTextObject("Test \n Text!", Colors::Red, font_id);

	// text can then be manipulated the same as a sprite
	text->SetPos(400, 100);
	text->SetZOrder(1);

	// For the collision system to be used it must first be enabled
	GameHelper::EnableCollisionSystem(true);

	// Then sprites can be added by getting the system
	auto collision_system = GameHelper::GetCollisionSystem();
	collision_system->CreateBoxCollider(sprite);

	// once added to the system the collisions for that sprite will be handled automatically

	// Alternatively sprites can also be removed from the system
	collision_system->DestroyBoxCollider(sprite->GetBoxCollider());


}

void Game::Update(float gameTime)
{
	// If the collision system is enabled all of the collision events can be queried each frame

	auto collision_events = GameHelper::GetCollisionSystem()->GetCollisionEvents();

	for (auto _event : collision_events)
	{
		if (_event.Contains(sprite))
		{
			// if the collision event involved the sprite it will return true
		}
		if (_event.ContainsBoth(sprite,atlas_sprite))
		{
			// if both of the provided sprites were involved in the collision then this will return true
		}
	}
}

void Game::Render(Renderer* renderer)
{
	// To render objects they must be added to the renderer here using
	// renderer->renderObject()

	renderer->renderObject(sprite);
	renderer->renderObject(atlas_sprite);
	renderer->renderObject(atlas_sprite2);

	// Text is rendered the same way
	renderer->renderObject(text);
}

void Game::KeyHandler(KeyEvent keyEvent, Mouse mouse)
{
	// Key codes are handled using GLFW and can be accessed using the glfw macros
	// such as GLFW_KEY_A

	// text can have its text changed via a call to the GameHelper

	if (keyEvent.key == GLFW_KEY_A)
	{
		GameHelper::ChangeText(text, "This is new \n Text with a different Color! \n  \n", Colors::Black, 0);
		text->SetPos(500, -50);
	}

	// the game can be ended using a call to the game helper aswell

	if (keyEvent.key == GLFW_KEY_ESCAPE)
	{
		GameHelper::StopGame();
	}
}

void Game::End()
{
}
