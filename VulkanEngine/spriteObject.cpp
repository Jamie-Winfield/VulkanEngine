#include "spriteObject.h"
#include "helpers.cpp"




SpriteObject::SpriteObject(VkDevice _device, Vector2 _screen, VkCommandPool _cmdpool, VkQueue _graphicsQueue,
    VkPhysicalDevice _physicalDevice, uint16_t _id) : Renderable(_device, _cmdpool, _graphicsQueue, _physicalDevice, _screen)
{
    sprite_id = _id;
}

SpriteObject::~SpriteObject()
{
    Free();
}

uint16_t SpriteObject::GetId()
{
    return sprite_id;
}

BoxCollider* SpriteObject::GetBoxCollider()
{
    return boxCollider;
}

void SpriteObject::SetBoxCollider(BoxCollider* _collider)
{
    boxCollider = _collider;
}

void SpriteObject::Update()
{
    if (boxCollider != nullptr)
    {
        boxCollider->UpdateBox(GetPosition(), GetRotation(), GetScale());
    }
}


