#include "Renderable.h"
#include "Helpers.cpp"

Renderable::Renderable(VkDevice _device, VkCommandPool _cmdpool, VkQueue _graphicsQueue,
    VkPhysicalDevice _physicalDevice, Vector2 _screen)
{
    device = _device;
    screen = _screen;
    physicalDevice = _physicalDevice;
    cmdPool = _cmdpool;
    graphicsQueue = _graphicsQueue;
}

void Renderable::Free()
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

void Renderable::AddIndex(uint16_t _index)
{
    indices.emplace_back(_index);
}

void Renderable::AddVertex(Vertex _vertex)
{
    vertices.emplace_back(_vertex);
}

void Renderable::SetTextureImageView(VkImageView _textureImageView)
{
    textureImageView = _textureImageView;
}

void Renderable::SetTextureImage(VkImage _image)
{
    textureImage = _image;
}

void Renderable::CreateVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
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

void Renderable::CreateIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue)
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

void Renderable::SetUVs(Vector2x2 _uv)
{
    const std::vector<Vertex> _vertices =
    {
        {{0.f, 1.f}, {1.0f, 0.0f, 0.0f, 1.f}, {_uv.y.x, _uv.x.y}},
        {{1.f, 1.f}, {0.0f, 1.0f, 0.0f, 1.f}, {_uv.x.x, _uv.x.y}},
        {{1.f, 0.f}, {0.0f, 0.0f, 1.0f, 1.f}, {_uv.x.x, _uv.y.y}},
        {{0.f, 0.f}, {1.0f, 1.0f, 1.0f, 1.f}, {_uv.y.x, _uv.y.y}}
    };

    vertices = _vertices;


    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Helpers::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory, device, physicalDevice);


    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(device, stagingBufferMemory);

    VkCommandBuffer commandBuffer = Helpers::beginSingleTimeCommands(device, cmdPool);

    vkCmdUpdateBuffer(commandBuffer, vertexBuffer, 0, bufferSize, data);

    Helpers::endSingleTimeCommands(commandBuffer, device, graphicsQueue,
        cmdPool);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

VkImage& Renderable::GetTextureImage()
{
    return textureImage;
}

VkImageView& Renderable::GetTextureImageView()
{
    return textureImageView;
}

void Renderable::SetPos(float _x, float _y)
{
    position.x = _x;
    position.y = _y;
    UpdateModelMatrix();
}

void Renderable::SetPos(Vector2 _pos)
{
    position = _pos;
    UpdateModelMatrix();
}

void Renderable::ChangePos(float _x, float _y)
{
    position.x += _x;
    position.y += _y;
    UpdateModelMatrix();
}

void Renderable::ChangePos(Vector2 _pos)
{
    position += _pos;
    UpdateModelMatrix();
}

void Renderable::SetRotation(float _rotation)
{
    rotation = _rotation;
    UpdateModelMatrix();
}

void Renderable::ChangeRotation(float _rotation)
{
    rotation += _rotation;
    UpdateModelMatrix();
}

void Renderable::FlipRenderable()
{
    if (!flipped)
    {
        flip_rotation = 180;
    }
    else
    {
        flip_rotation = 0;
    }
    flipped = !flipped;
    UpdateModelMatrix();
}

void Renderable::SetFlipped(bool _flipped)
{
    flipped = _flipped;
    if (!flipped)
    {
        flip_rotation = 180;
    }
    else
    {
        flip_rotation = 0;
    }
    UpdateModelMatrix();
}

void Renderable::SetZOrder(float _z)
{
    z_order = _z;
    UpdateModelMatrix();
}

void Renderable::SetScale(float _x, float _y)
{
    scale.x = _x;
    scale.y = _y;
    UpdateModelMatrix();
}

void Renderable::SetScale(Vector2 _scale)
{
    scale = _scale;
    UpdateModelMatrix();
}

float Renderable::GetXPosition()
{
    return position.x;
}

float Renderable::GetYPosition()
{
    return position.y;
}

float Renderable::GetZOrder()
{
    return z_order;
}

float Renderable::GetWidth()
{
    return scale.x;
}

float Renderable::GetHeight()
{
    return scale.y;
}

float Renderable::GetRotation()
{
    return rotation;
}

bool Renderable::GetFlipped()
{
    return flipped;
}

Vector2 Renderable::GetPosition()
{
    return position;
}

Vector2 Renderable::GetScale()
{
    return scale;
}

glm::mat4 Renderable::GetModelMatrix()
{
    return model_matrix;
}

VkBuffer& Renderable::GetVertexBuffer()
{
    return vertexBuffer;
}

VkBuffer& Renderable::GetIndexBuffer()
{
    return indexBuffer;
}

std::vector<uint16_t>& Renderable::GetIndices()
{
    return indices;
}

void Renderable::UpdateModelMatrix()
{
    model_matrix = glm::translate(glm::mat4(1.f), glm::vec3(position.x, position.y, z_order)) *
        glm::rotate(glm::mat4(1.f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) * 
        glm::rotate(glm::mat4(1.f), glm::radians(flip_rotation), glm::vec3(0.f, 1.f, 0.f)) *
        glm::scale(glm::mat4(1.f), glm::vec3(scale.x, scale.y, 1));
    updated1 = true;
    updated2 = true;
    updated3 = true;
    Update();
}
