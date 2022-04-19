#pragma once
#include <vulkan/vulkan.hpp>
#include "Vertex.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "uniformBufferObject.h"
#include "BoxCollider.h"
#include "HelperTypes.h"

#include "Renderable.h"



class SpriteObject : public Renderable
{
public:
    

    bool operator== (SpriteObject& _object)
    {
        return _object.GetId() == sprite_id;
    }

    
    
private:
    uint16_t sprite_id = 0;
    BoxCollider* boxCollider = nullptr;
public:

    SpriteObject(VkDevice _device, Vector2 _screen, VkCommandPool _cmdpool, VkQueue _graphicsQueue,
        VkPhysicalDevice _physicalDevice, uint16_t _id);
    ~SpriteObject();

    uint16_t GetId();
    BoxCollider* GetBoxCollider();

    void SetBoxCollider(BoxCollider* _collider);

private:

    void Update() override;
    
    

    

};
