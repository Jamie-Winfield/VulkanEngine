#pragma once
#include <cstdint>
#include <vector>

struct Settings
{
	const uint32_t W_WIDTH = 800;
	const uint32_t W_HEIGHT = 800;

	const std::vector<const char*> validationLayers = 
	{
		"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif

	const int MAX_FRAMES_IN_FLIGHT = 2;

};