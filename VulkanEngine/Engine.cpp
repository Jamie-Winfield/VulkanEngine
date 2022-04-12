#include "Engine.h"
#include "helpers.cpp"

SpriteObject* Engine::createSprite(const char* filename)
{
    std::unique_ptr<SpriteObject> spriteObject;
    spriteObject = std::make_unique<SpriteObject>(device, settings->W_WIDTH,settings->W_HEIGHT, current_sprite_id);
    current_sprite_id++;

    const std::vector<Vertex> vertices =
    {
        {{0.f, 1.f}, {1.0f, 0.0f, 0.0f}, {1.f, 0.f}},
        {{1.f, 1.f}, {0.0f, 1.0f, 0.0f}, {0.f, 0.f}},
        {{1.f, 0.f}, {0.0f, 0.0f, 1.0f}, {0.f, 1.f}},
        {{0.f, 0.f}, {1.0f, 1.0f, 1.0f}, {1.f, 1.f}}
    };

    const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
    };

    for (auto &vertex : vertices)
    {
        spriteObject->addVertex(vertex);
    }
    for (auto& index : indices)
    {
        spriteObject->addIndex(index);
    }
    spriteObject->createVertexBuffer(device, physicalDevice, renderer->getCommandPool(), graphicsQueue);
    spriteObject->createIndexBuffer(device, physicalDevice, renderer->getCommandPool(), graphicsQueue);
    spriteObject->textureImage = renderer->createTextureImage(device, physicalDevice, filename, graphicsQueue);
    spriteObject->setTextureImageView(renderer->createImageView(spriteObject->textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, device));

    createdSprites.emplace_back(std::move(spriteObject));
    return createdSprites.back().get();
}

void Engine::initVulkan(Settings* _settings, GLFWwindow* _window)
{
	settings = _settings;
    window = _window;
	createInstance();
    debugMessenger.setupDebugMessenger(settings, instance);
    createSurface(window);
    pickPhysicalDevice();
    createLogicalDevice();
    renderer = std::make_unique<Renderer>();
    renderer->init(settings,device,physicalDevice,surface,window);
}

void Engine::createInstance()
{
    if (settings->enableValidationLayers && !checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (settings->enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(settings->validationLayers.size());
        createInfo.ppEnabledLayerNames = settings->validationLayers.data();

        debugMessenger.populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }


    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}

bool Engine::checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : settings->validationLayers)
    {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

std::vector<const char*> Engine::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (settings->enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void Engine::createSurface(GLFWwindow* window)
{
    auto result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface! VkResult code:" + std::to_string(result));
    }
}

void Engine::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if (deviceCount <= 0)
    {
        throw std::runtime_error("Failed to find any GPU with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    std::multimap<int, VkPhysicalDevice> candidates;
    for (const auto& device : devices)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);
        std::string deviceName = deviceProperties.deviceName;
        std::cout << "Physical device called: " + deviceName + " detected!" << std::endl;
        int score = scoreDevice(device);
        candidates.insert(std::make_pair(score, device));
    }
    // Check if the best candidate is suitable at all
    for (const auto& candidate : candidates)
    {

        if (candidate.first > 0)
        {
            if (isDeviceSuitable(candidate.second))
            {
                physicalDevice = candidate.second;
            }
        }
    }
    if (physicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

uint16_t Engine::scoreDevice(VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceProperties deviceProperties;        // used to check basic properties such as name, type and supported vulkan version
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    VkPhysicalDeviceFeatures deviceFeatures;            // used to check more advanced features such as texture compression, 64 bit floats and multi-viewport rendering
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

    uint16_t score = 0;

    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) // discrete gpus have better preformance
    {
        score += 1000;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;

    if (!deviceFeatures.geometryShader) {
        return 0;
    }
    return score;
}

bool Engine::isDeviceSuitable(VkPhysicalDevice physicalDevice)
{
    // Add checks for device requirements here

    QueueFamilyIndices indices = Helpers::findQueueFamilies(physicalDevice, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

    bool swapChainAdequate = false;

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = Helpers::querySwapChainSupport(physicalDevice, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}



void Engine::updateRenderables(std::vector<SpriteObject*> objects)
{
    renderer->updateRenderables(objects, window, device, physicalDevice, surface);
}

void Engine::cleanup()
{
    renderer->cleanup(device);

    

    for (auto& sprite : createdSprites)
    {
        if (sprite != nullptr)
        {
            sprite->free();
            sprite.reset(nullptr);
        }
    }

    vkDestroyDevice(device, nullptr);

    if (settings->enableValidationLayers)
    {
        debugMessenger.DestroyDebugUtilsMessengerEXT(instance, nullptr);
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);

    vkDestroyInstance(instance, nullptr);
}

void Engine::StopGame()
{
    glfwSetWindowShouldClose(window, 1);
}

bool Engine::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}



void Engine::createLogicalDevice()
{
    QueueFamilyIndices indices = Helpers::findQueueFamilies(physicalDevice, surface);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.f;

    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }


    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();



    if (settings->enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(settings->validationLayers.size());
        createInfo.ppEnabledLayerNames = settings->validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }
    auto result = vkCreateDevice(physicalDevice, &createInfo, nullptr, &device);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device! VkResult code: " + std::to_string(result));
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &presentQueue);
}

