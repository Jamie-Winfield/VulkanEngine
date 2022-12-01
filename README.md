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

# Collision System

To start using the built in collision system it first must be enabled through the game helper.

```C++
GameHelper::EnableCollisionSystem(true);
```

Then sprites which should have their collisions tracked just need to be added to the system
```C++
auto collision_system = GameHelper::GetCollisionSystem();
collision_system->CreateBoxCollider(sprite);

// Collisions between sprites added to the system are handled automatically.

// Sprites can also be removed from the system easily.
collision_system->DestroyBoxCollider(sprite->GetBoxCollider());
```

While collisions for added sprites are automatically handled the collision events are also provided every frame
```C++
auto collision_events = GameHelper::GetCollisionSystem()->GetCollisionEvents();
for (auto& _event : collision_events)
{
	if(_event.Contains(sprite))
	{
		// if the event is between the provided sprite and another this will be executed
	}
	if(_event.ContainsBoth(sprite, sprite2))
	{
		// if both of the provided sprites were involved in the event then this will be executed
	}
}
```

If you just need the collision events and don't need sprites to be moved when they collide with eachother then you can set the sprites to not be moveable

```C++
sprite->GetBoxCollider()->SetMoveable(false);
```

# Exiting the Engine

To quit the game there is a simple function which can be called from the Game helper

```C++
GameHelper::StopGame();
```

# Known Bugs

There is a bug with the text system on some graphics cards which causes the engine to crash.


