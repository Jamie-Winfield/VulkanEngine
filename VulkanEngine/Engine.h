#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <memory>
#include <string>
#include <cstdint> // Necessary for UINT32_MAX
#include <algorithm> // Necessary for std::clamp

#include "settings.hpp"
#include "Vertex.h"
#include "spriteObject.h"
#include "uniformBufferObject.h"
#include "DebugMessenger.h"
#include "Renderer.h"
#include "queueFamilyIndices.h"
#include "swapChainSupportDetails.h"

#include <map>
#include <set>
#include <optional>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "spriteObject.h"

#include <chrono>





class Engine
{
public:

	SpriteObject* createSprite(const char* filename);
	void initVulkan(Settings* _settings, GLFWwindow* _window);
	VkDevice getDevice() { return device; }
	VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
	GLFWwindow* getWindow() { return window; }
	Settings* getSettings() { return settings; }
	VkQueue getGraphicsQueue() { return graphicsQueue; }
	VkQueue getPresentQueue() { return presentQueue; }
	Renderer* getRenderer() { return renderer.get(); }
	VkSurfaceKHR getSurface() { return surface; }
	void updateRenderables(std::vector<SpriteObject*> objects);
	void cleanup();

private:
	void createInstance();
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void createSurface(GLFWwindow* window);
	void pickPhysicalDevice();
	uint16_t scoreDevice(VkPhysicalDevice physicalDevice);
	bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
	
	bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
	
	void createLogicalDevice();





private:
	GLFWwindow* window;
	Settings* settings;
	VkInstance instance;
	DebugMessenger debugMessenger;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	std::unique_ptr<Renderer> renderer;
	std::vector<std::unique_ptr<SpriteObject>> createdSprites;
};

