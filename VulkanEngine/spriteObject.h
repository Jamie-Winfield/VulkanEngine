#pragma once
#include <vulkan/vulkan.hpp>
#include "Vertex.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "uniformBufferObject.h"



class SpriteObject
{

    
private:
    std::vector<Vertex> vertices;
    
    
    VkDeviceMemory vertexBufferMemory;
    
    VkDeviceMemory indexBufferMemory;
public:

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    std::vector<uint16_t> indices;

    std::vector<VkDescriptorSet> descriptorSets;

    bool freed = false;

    glm::mat4 modelMatrix = glm::mat4(1.f);

private:


    float pos_x = 0;
    float pos_y = 0;
    float pos_z = 0;
    float rot_x = 0;
    float rot_y = 0;
    float rot_z = 0;

    float scale_x = 1;
    float scale_y = 1;
    float scale_z = 1;

    float rot_angle = 0;

   

    void updateModelMatrix()
    {
        modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(pos_x, pos_y, pos_z)) * 
            glm::rotate(glm::mat4(1.f),glm::radians(rot_angle),glm::vec3(0.f,0.f,1.f)) *
            glm::scale(glm::mat4(1.f),glm::vec3(scale_x,scale_y,scale_z));
    }

public:
    float getAngle() { return rot_angle; }

    float getXPos() { return pos_x; }
    float getYPos() { return pos_y; }

    void setScale(float x, float y)
    {
        scale_x = x;
        scale_y = y;
        updateModelMatrix();
    }

    void setRotation(float rot)
    {
        rot_angle = rot;
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

    void free(VkDevice device)
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

            //vkDestroyImageView(device, textureImageView, nullptr);
            //vkDestroyImage(device, textureImage, nullptr);
            //vkFreeMemory(device, textureImageMemory, nullptr);

            freed = true;
        }
    }
};
