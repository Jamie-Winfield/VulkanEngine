#pragma once
#include <vulkan/vulkan.hpp>
#include "Vertex.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "uniformBufferObject.h"
#include "BoxCollider.h"
#include "HelperTypes.h"



class SpriteObject
{
public:
    
    BoxCollider::Quad quad = BoxCollider::Quad::ALL;

    bool operator== (SpriteObject& _object)
    {
        return _object.GetId() == sprite_id;
    }

    
    
private:
    std::vector<Vertex> vertices;
    
    
    VkDeviceMemory vertexBufferMemory;
    
    VkDeviceMemory indexBufferMemory;

    VkDevice device;

    uint16_t sprite_id = 0;
public:

    SpriteObject(VkDevice _device, float _screenx, float _screeny, uint16_t _id);
    ~SpriteObject();

    uint16_t GetId() { return sprite_id; }

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    std::vector<uint16_t> indices;

    

    std::vector<VkDescriptorSet> descriptorSets;

    bool freed = false;

    

    bool updated1 = true;
    bool updated2 = true;
    bool updated3 = true;



    float rot_angle = 0;

    glm::mat4 modelMatrix = glm::mat4(1.f);


    BoxCollider* GetBoxCollider()
    {
        return boxCollider;
    }

    void SetBoxCollider(BoxCollider* _collider)
    {
        boxCollider = _collider;
    }

private:

    float screen_x = 0;
    float screen_y = 0;


    float pos_x = 0;
    float pos_y = 0;
    float pos_z = 0;

    Vector2 position;

    float rot_x = 0;
    float rot_y = 0;
    float rot_z = 0;

    float scale_x = 1;
    float scale_y = 1;
    float scale_z = 1;

    Vector2 scale;

    bool flipped = false;
    float flip = 0;


    
    BoxCollider* boxCollider = nullptr;

    


    

    void SetQuad();

    void updateModelMatrix()
    {
        modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(pos_x, pos_y, pos_z)) * 
            glm::rotate(glm::mat4(1.f),glm::radians(rot_angle),glm::vec3(0.f,0.f,1.f)) * glm::rotate(glm::mat4(1.f), glm::radians(flip), glm::vec3(0.f, 1.f, 0.f)) *
            glm::scale(glm::mat4(1.f),glm::vec3(scale_x,scale_y,scale_z));
        SetQuad();
        updated1 = true;
        updated2 = true;
        updated3 = true;
        position = { pos_x,pos_y };
        scale = { scale_x,scale_y };
        if (boxCollider != nullptr)
        {
            boxCollider->UpdateBox(position, rot_angle, scale);
        }
    }

public:
    float GetRotation() { return rot_angle; }

    float getXPos() { return pos_x; }
    float getYPos() { return pos_y; }
    float GetWidth() { return scale_x; }
    float GetHeight() { return scale_y; }
    Vector2 GetPos() { return position; }
    Vector2 GetScale() { return scale; }

    void setScale(float x, float y)
    {
        scale_x = x;
        scale_y = y;
        updateModelMatrix();
    }

    /// <summary>
    /// Stops the sprite from showing up in the scene by flipping it 180deg, doesn't effect preformance
    /// however offers alternative to changing renderables list which does effect preformance
    /// </summary>
    void FlipSprite()
    {
        if (!flipped)
        {
            flip = 180;
        }
        else
        {
            flip = 0;
        }
        flipped = !flipped;
        updateModelMatrix();
    }

    void setRotation(float rot)
    {
        rot_angle = rot;
        updateModelMatrix();
    }

    void Rotate(float _rot)
    {
        rot_angle += _rot;
        updateModelMatrix();
    }

    void updatePos(float x, float y, float z)
    {

        pos_x += x;
        pos_y += y;
        pos_z += z;
        updateModelMatrix();
    }

    void setPos(float x, float y, float z)
    {
        pos_x = x;
        pos_y = y;
        pos_z = z;
        updateModelMatrix();

    }

    void setTextureImageView(VkImageView _textureImageView) { textureImageView = _textureImageView; }

    void addVertex(Vertex vertex)
    {
        vertices.emplace_back(vertex);
    }

    void createVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);

    void createIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);

    void addIndex(uint16_t index)
    {
        indices.emplace_back(index);
    }

    void free()
    {
        if (!freed)
        {
            if (vertexBuffer != VK_NULL_HANDLE)
            {
                vkDestroyBuffer(device, vertexBuffer, nullptr);
            }
            if (vertexBufferMemory != VK_NULL_HANDLE)
            {
                vkFreeMemory(device, vertexBufferMemory, nullptr);
            }
            if (indexBuffer != VK_NULL_HANDLE)
            {
                vkDestroyBuffer(device, indexBuffer, nullptr);
            }
            if (indexBufferMemory != VK_NULL_HANDLE)
            {
                vkFreeMemory(device, indexBufferMemory, nullptr);
            }
            freed = true;
        }
    }
};
