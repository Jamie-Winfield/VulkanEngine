#pragma once
#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_SWIZZLE
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
#include "CollisionSystem.h"

#include <map>
#include <set>
#include <optional>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "spriteObject.h"


#include <chrono>





class Engine
{
public:


	SpriteObject* createSprite(const char* filename);
	Renderable* AddCreatedSprite(std::unique_ptr<Renderable> _renderable);
	void initVulkan(Settings* _settings, GLFWwindow* _window);
	VkDevice getDevice() { return device; }
	VkPhysicalDevice getPhysicalDevice() { return physicalDevice; }
	GLFWwindow* getWindow() { return window; }
	Settings* getSettings() { return settings; }
	VkQueue getGraphicsQueue() { return graphicsQueue; }
	VkQueue getPresentQueue() { return presentQueue; }
	Renderer* getRenderer() { return renderer.get(); }
	CollisionSystem* GetCollisionSystem() { return collision_system.get(); }
	VkSurfaceKHR getSurface() { return surface; }
	void updateRenderables(std::vector<Renderable*> objects);
	void cleanup();
	void StopGame();

	float GetWindowWidth() { return window_width; }
	float GetWindowHeight() { return window_height; }
	Vector2 GetScreenSize() { return Vector2(window_width, window_height); }

	void EnableCollisionSystem();
	void DisableCollisionSystem();

	bool GetFrameCounterEnable();

	void SetFrameCounterEnable(bool _enable);

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
	float window_width = 0;
	float window_height = 0;
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
	std::vector<std::unique_ptr<Renderable>> createdSprites;

	uint16_t current_sprite_id = 0;

	std::unique_ptr<CollisionSystem> collision_system;

	bool frameCounter = false;
	
};

