#pragma once
#include "HelperTypes.h"
#include "Vertex.h"
#include "uniformBufferObject.h"
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan.hpp>

class Renderable
{
private:
	Vector2 position = Vector2(0,0);
	float z_order = 0;
	float rotation = 0;
	Vector2 scale = Vector2(1, 1);
	bool flipped = false;
	float flip_rotation = 0;
	Vector2 screen = Vector2(0, 0);

	glm::mat4 model_matrix = glm::mat4(1.f);

	

	bool freed = false;

	std::vector<Vertex> vertices;


	VkDeviceMemory vertexBufferMemory;

	VkDeviceMemory indexBufferMemory;

	VkDevice device;
	VkPhysicalDevice physicalDevice;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;
	std::vector<uint16_t> indices;

	

	VkCommandPool cmdPool;
	VkQueue graphicsQueue;

public:
	std::vector<VkDescriptorSet> descriptorSets;

	bool updated1 = true;
	bool updated2 = true;
	bool updated3 = true;


private:
	void UpdateModelMatrix();

	void virtual Update() = 0;



public:

	Renderable(VkDevice _device, VkCommandPool _cmdpool, VkQueue _graphicsQueue, VkPhysicalDevice _physicalDevice, Vector2 _screen);

	void Free();
	void AddIndex(uint16_t _index);
	void AddVertex(Vertex _vertex);
	void SetTextureImageView(VkImageView _textureImageView);
	void SetTextureImage(VkImage _image);
	void CreateVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);
	void CreateIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue);
	void SetUVs(Vector2x2 _uv);

	VkImage& GetTextureImage();
	VkImageView& GetTextureImageView();

	void SetPos(float _x, float _y);
	void SetPos(Vector2 _pos);
	void ChangePos(float _x, float _y);
	void ChangePos(Vector2 _pos);

	void SetRotation(float _rotation);
	void ChangeRotation(float _rotation);

	void FlipRenderable();
	void SetFlipped(bool _flipped);

	void SetZOrder(float _z);

	void SetScale(float _x, float _y);
	void SetScale(Vector2 _scale);

	float GetXPosition();
	float GetYPosition();
	float GetZOrder();
	float GetWidth();
	float GetHeight();
	float GetRotation();
	bool GetFlipped();
	Vector2 GetPosition();
	Vector2 GetScale();

	glm::mat4 GetModelMatrix();
	VkBuffer& GetVertexBuffer();
	VkBuffer& GetIndexBuffer();
	std::vector<uint16_t>& GetIndices();

	
};

