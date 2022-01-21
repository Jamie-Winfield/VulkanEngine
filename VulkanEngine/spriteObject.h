#pragma once
#include <vulkan/vulkan.hpp>
#include "Vertex.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "uniformBufferObject.h"

struct SpriteObject
{

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    bool freed = false;



    float pos_x = 0;
    float pos_y = 0;
    float pos_z = 0;
    float rot_x = 0;
    float rot_y = 0;
    float rot_z = 0;

    float rot_angle = 0;

    glm::mat4 modelMatrix = glm::mat4(1.f);

    void updateModelMatrix()
    {
        modelMatrix = glm::translate(glm::mat4(1.f), glm::vec3(pos_x, pos_y, pos_z)) * glm::rotate(glm::mat4(1.f),glm::radians(rot_angle),glm::vec3(0.f,0.f,1.f));
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
            freed = true;
        }
    }
};
