#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

struct Settings
{
	uint32_t W_WIDTH = 800;
	uint32_t W_HEIGHT = 800;

	std::string window_name = "";

	const std::vector<const char*> validationLayers = 
	{
		"VK_LAYER_KHRONOS_validation"
	};

	float backgroundColorRed = 0.f;
	float backgroundColorGreen = 0.f;
	float backgroundColorBlue = 1.f;

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	const int MAX_FRAMES_IN_FLIGHT = 2;

};