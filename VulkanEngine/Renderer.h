#pragma once
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <cstdint>
#include "uniformBufferObject.h"
#include "spriteObject.h"
#include "settings.hpp"
#include "Camera.h"
#include "Renderable.h"




class Renderer
{
public:

	

	void init(Settings* _settings, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window);
	
	void drawFrame(VkDevice device, VkQueue graphicsQueue, VkQueue presentQueue,
		GLFWwindow* window, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	
	void cleanup(VkDevice device);

	void updateRenderables(std::vector<Renderable*> objects, GLFWwindow* window, VkDevice device,
		VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void renderObject(Renderable* spriteObject);
	VkCommandPool getCommandPool() { return commandPool; }
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
		VkDevice device, VkQueue graphicsQueue);
	VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice device);
	std::pair<VkImage, Vector2> createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice,
		const char* filename, VkQueue graphicsQueue);
	std::pair<VkImage, Vector2> CreateAtlasImage(VkDevice device, VkPhysicalDevice physicalDevice,
		uint32_t width, uint32_t height, const char* atlasname, VkQueue graphicsQueue);

	unsigned char* GetPixels(const char* filename, int& texWidth, int& texHeight, int& texChannels);

	void FreePixels(unsigned char* pixels);

	Camera* GetCamera();

	VkCommandPool GetCommandPool() { return commandPool; }
	
private:
	void createSwapChain(VkPhysicalDevice PhysicalDevice, VkDevice device, VkSurfaceKHR surface, GLFWwindow* window);
	void createImageViews(VkDevice device);
	void createRenderPass(VkDevice device, VkPhysicalDevice physicalDevice);
	void createDescriptorSetLayout(VkDevice device);
	void createGraphicsPipeline(VkDevice device);
	void createFrameBuffers(VkDevice device);
	void createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void createUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice);
	void createDescriptorPool(VkDevice device);
	void createDescriptorSets(VkDevice device, std::vector<VkDescriptorSet>& _descriptorSets, VkImageView textureImageView);
	void createCommandBuffers(VkDevice device);
	void createSyncObjects(VkDevice device);
	void updateUniformBuffers(uint32_t currentImage, VkDevice device);
	void recreateSwapChain(GLFWwindow* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	void cleanupSwapChain(VkDevice device);
	void recreateBuffers(VkDevice device, VkPhysicalDevice physicalDevice);
	void createDepthResources(VkPhysicalDevice physicalDevice, VkDevice device);
	void CreateCamera();

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice);
	VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);
	bool hasStencilComponent(VkFormat format);

	void createTextureSampler(VkPhysicalDevice physicalDevice, VkDevice device);
	
	VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresetModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

private:
	VkSurfaceKHR surface;
	VkSwapchainKHR swapChain;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VkDescriptorPool descriptorPool;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;
	Settings* settings;
	VkSampler textureSampler;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	std::vector<std::tuple<std::string, VkImage, VkDeviceMemory, Vector2>> images;
	std::vector<std::pair<VkImage, VkImageView>> imageViews;
	std::vector<std::pair<VkImageView,std::vector<VkDescriptorSet>>> descriptorSets;

	std::unique_ptr<Camera> camera;


public:
	bool framebufferResized = false;
	

	std::vector<Renderable*> spriteObjects;
	std::vector<Renderable*> spriteObjects_new;
};

