#include "spriteObject.h"
#include "helpers.cpp"




SpriteObject::SpriteObject(VkDevice _device, float _screenx, float _screeny)
{
    device = _device;
    screen_x = _screenx;
    screen_y = _screeny;
    SetQuad();
}

SpriteObject::~SpriteObject()
{
    free();
}

void SpriteObject::SetQuad()
{
    // bottom left = 2
    // bottom right = 3
    // top left = 4
    // top right = 5

    int quad_int = 0;

    // four corners of cube
    int p_1 = 0;
    int p_2 = 0;
    int p_3 = 0;
    int p_4 = 0;

    // bottom left corner
    if (pos_x >= screen_x / 2)
    {
        if (pos_y >= screen_y / 2)
        {
            p_1 = 5;
        }
        else
        {
            p_1 = 3;
        }
    }
    else if (pos_y >= screen_y / 2)
    {
        p_1 = 4;
    }
    else
    {
        p_1 = 2;
    }

    // top left corner
    if (pos_x >= screen_x / 2)
    {
        if (pos_y + scale_y >= screen_y / 2)
        {
            p_3 = 5;
        }
        else
        {
            p_3 = 3;
        }
    }
    else if (pos_y + scale_y >= screen_y / 2)
    {
        p_3 = 4;
    }
    else
    {
        p_3 = 2;
    }

    // top right corner
    if (pos_x + scale_x >= screen_x / 2)
    {
        if (pos_y + scale_y >= screen_y / 2)
        {
            p_4 = 5;
        }
        else
        {
            p_4 = 3;
        }
    }
    else if (pos_y + scale_y >= screen_y / 2)
    {
        p_4 = 4;
    }
    else
    {
        p_4 = 2;
    }

    // bottom right corner
    if (pos_x + scale_x >= screen_x / 2)
    {
        if (pos_y >= screen_y / 2)
        {
            p_2 = 5;
        }
        else
        {
            p_2 = 3;
        }
    }
    else if (pos_y >= screen_y / 2)
    {
        p_2 = 4;
    }
    else
    {
        p_2 = 2;
    }

    quad_int = p_1;
    if (p_2 != p_1)
    {
        quad_int *= p_2;
    }
    if (p_3 != p_2 && p_3 != p_1)
    {
        quad_int *= p_3;
    }
    if (p_4 != p_3 && p_4 != p_2 && p_4 != p_1)
    {
        quad_int *= p_4;
    }

    quad = static_cast<Quad>(quad_int);

}

void SpriteObject::createVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
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

void SpriteObject::createIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
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
