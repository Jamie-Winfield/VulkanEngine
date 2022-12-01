# VulkanEngine

This is a simple 2D game engine using the Vulkan API and compiled with Visual Studio Build tools 2019.

For examples on how to use this engine check out the examples branch and for a simple completed game of pong check out the example game branch.

# Setup

To utilise the engine firstly clone the repository and use the included game.cpp / .hpp for game development, these files contain all of the boiler plate code required to create a game.

# Examples

# Sprites and Text Creation
There are two ways to create a sprite in the engine,
Firstly simply a texture can be used with a .png or .jpg format.

```C++
  SpriteObject* sprite = GameHelper::CreateSprite("textures/texture.jpg");
```

Alternitively you can create a texture atlas to store multiple textures for sprites.

```C++
// Each texture atlas should be given a unique name so that it can be looked up in the gamehelper if needed later.
// The atlas also just needs a size to be created through the game helper
TextureAtlas* atlas = GameHelper::CreateTextureAtlas(1000, 1000, "atlas1");

// next textures can be added to the atlas
atlas->AddImage("textures/texture.jpg");
atlas->AddImage("textures/whitepix.png");
atlas->AddImage("textures/NoTexture.png");
// images are automatically placed in the atlas, if there is insufficient space
// an error will be thrown
  
// next the atlas can be assigned to a sprite the same way as normally creating a sprite  
SpriteObject* atlas_sprite = GameHelper::CreateSprite("atlas1");

// Finally the actual image from the atlas can be set on the sprite using its UVs
atlas_sprite->SetUVs(atlas->GetUV("textures/texture.jpg"));

// Multiple sprites can use the same atlas
SpriteObject* atlas_sprite2 = GameHelper::CreateSprite("atlas1");
atlas_sprite2->SetUVs(atlas->GetUV("textures/whitepix.png"));

```

The SpiteObject class offers multiple functions to control a sprites position, size and scale.

```C++
// the scale of a sprite is by default set to the size of the texture provided
// however can be changed using SetScale where the width and height provided are its size in pixels
sprite->SetScale(100, 100);

// The sprite can then be positioned
sprite->SetPos(200, 200);

// The sprite can also have its z order set
sprite->SetZOrder(1);
```

To create text, firstly a font needs to be loaded into the engine the font must be in a .ttf format.
```C++
auto font_loader = GameHelper::GetFontLoader();

// Each font when created will be given a unique id so that it can be referenced in text later
auto font_id = font_loader->CreateFont("fonts/SalmaproMedium-0Wooo.ttf", 25);

```

Next the actual text can be created.
```C++
Text* text = GameHelper::CreateTextObject("Test \n Text!", Colors::Red, font_id);
```

Text can be manipulated similar to how sprites are.
```C++
text->SetPos(400, 100);
text->SetZOrder(1);

// Scaling can cause the rendered text to display incorrecly but is still possible
text->SetScale(100,100);
```
Text can be changed using a call to the game helper class
```C++
GameHelper::ChangeText(text, "This is new \n Text with a different Color! \n  \n", Colors::Black, 0);
```

# Rendering

Rendering takes place in the Render function, render calls outside of this function can cause undefined behaviour.
Sprites and Text are both rendered the same way
```C++
void Game::Render(Renderer* renderer)
{
 	renderer->renderObject(sprite);
	renderer->renderObject(text);
}
```

# Key Handling

Key and mouse events are handled using GLFW and thus can be compared using GLFW macros
All key and mouse events for the previous frame are provided via the KeyHandler function

keyEvent contains all the information for the key which is currently being processed, this includes mouse presses.
mouse contains the x and y position of the mouse on the screen.
```C++
void Game::KeyHandler(KeyEvent keyEvent, Mouse mouse)
{
	if (keyEvent.key == GLFW_KEY_A && keyEvent.action == KeyEvent::ActionCode::KEY_PRESSED)
	{
		// do something if the a key was pressed last frame
	}
	else if (keyEvent.key == GLFW_KEY_B && keyEvent.action == KeyEvent::ActionCode::KEY_RELEASED)
	{
		// do something else if the b key was released last frame
	}
	else if(keyEvent.key == GLFW_MOUSE_BUTTON_RIGHT && keyEvent.action == KeyEvent::ActionCode::KEY_PRESSED)
	{
		// do something else if the right mouse button was pressed
	}
}
```








