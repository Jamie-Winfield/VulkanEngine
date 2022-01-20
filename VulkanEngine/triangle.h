#pragma once
#include <vulkan/vulkan.hpp>
#include "Vertex.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "uniformBufferObject.h"

struct Triangle
{
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    std::vector<VkDescriptorSet> descriptorSets;

    void createUniformBuffers(size_t size, VkDevice& device, VkPhysicalDevice& physicalDevice)
    {
        VkDeviceSize bufferSize = sizeof(UniformBufferObject);
        uniformBuffers.resize(size);
        uniformBuffersMemory.resize(size);

        for (size_t i = 0; i < size; ++i)
        {
            Helpers::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i], device, physicalDevice);
        }
    }

    void createDescriptorSets(size_t size, VkDescriptorSetLayout descriptorSetLayout,VkDescriptorPool descriptorPool, VkDevice device)
    {
        std::vector<VkDescriptorSetLayout> layouts(size, descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(size);
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(size);

        auto result = vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data());
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate descriptor sets! vk error: " + std::to_string(result));
        }

        for (size_t i = 0; i < size; ++i)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;
            descriptorWrite.pImageInfo = nullptr;
            descriptorWrite.pTexelBufferView = nullptr;

            vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
        }
    }

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

    void addVertex(Vertex vertex)
    {
        vertices.emplace_back(vertex);
    }

    void createVertexBuffer(VkDevice& device, VkPhysicalDevice physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue)
    {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        Helpers::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);


        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device, stagingBufferMemory);

        Helpers::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory, device, physicalDevice);
        Helpers::copyBuffer(stagingBuffer, vertexBuffer, bufferSize, commandPool, graphicsQueue, device);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void createIndexBuffer(VkDevice& device, VkPhysicalDevice physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue)
    {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        Helpers::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, device, physicalDevice);


        void* data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device, stagingBufferMemory);

        Helpers::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory, device, physicalDevice);
        Helpers::copyBuffer(stagingBuffer, indexBuffer, bufferSize, commandPool, graphicsQueue, device);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

    void addIndex(uint16_t index)
    {
        indices.emplace_back(index);
    }

    void free(VkDevice& device)
    {
        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);
        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);
    }
};
