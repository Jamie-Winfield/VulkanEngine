#pragma once
#include <vulkan/vulkan.hpp>
#include <utility>
class TextureLoader
{
private:
	std::vector<std::tuple<char*, VkImage, VkDeviceMemory>> images;

public:
	VkImage createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice,
		const char* filename, VkImage& textureImage, VkDeviceMemory& textureImageMemory, VkQueue graphicsQueue);
};

