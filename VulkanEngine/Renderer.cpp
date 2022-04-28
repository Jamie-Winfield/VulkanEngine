#include "Renderer.h"
#include "helpers.cpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

void Renderer::init(Settings* _settings, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, GLFWwindow* window)
{
    settings = _settings;
    createSwapChain(physicalDevice, device, surface, window);
    createImageViews(device);
    createDepthResources(physicalDevice,device);
    createRenderPass(device, physicalDevice);
    createDescriptorSetLayout(device);
    createGraphicsPipeline(device);
    createFrameBuffers(device);
    createCommandPool(device, physicalDevice, surface);
    createTextureSampler(physicalDevice,device);
    createUniformBuffers(device, physicalDevice);
    createDescriptorPool(device);
    for (auto& sprite : spriteObjects)
    {
        createDescriptorSets(device, sprite->descriptorSets, sprite->GetTextureImageView());
    }
    createCommandBuffers(device);
    createSyncObjects(device);
    CreateCamera();
}

unsigned char* Renderer::GetPixels(const char* filename, int& texWidth, int& texHeight, int& texChannels)
{
    stbi_uc* pixels = stbi_load(filename, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    return pixels;
}

void Renderer::ResetVKImage(VkImage _image, VkImageView _imageview, VkDevice _device)
{
    for (int i = 0; i < imageViews.size(); ++i)
    {
        if (imageViews[i].second == _imageview)
        {
            vkDestroyImageView(_device, imageViews[i].second, nullptr);
            imageViews.erase(imageViews.begin() + i);
            break;
        }
    }
    for (int i = 0; i < images.size(); ++i)
    {
        if (std::get<1>(images[i]) == _image)
        {
            vkFreeMemory(_device, std::get<2>(images[i]), nullptr);
            vkDestroyImage(_device, std::get<1>(images[i]), nullptr);
            images.erase(images.begin() + i);
            break;
        }
    }
    
    
}

void Renderer::ResetCommandBuffers(VkDevice _device, GLFWwindow* window, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    recreateSwapChain(window, _device, physicalDevice, surface);
    camera->updated1 = true;
    camera->updated2 = true;
    camera->updated3 = true;

}

void Renderer::FreePixels(unsigned char* pixels)
{

}

void Renderer::createDepthResources(VkPhysicalDevice physicalDevice, VkDevice device)
{
    VkFormat depthFormat = findDepthFormat(physicalDevice);

    Helpers::createImage(swapChainExtent.width, swapChainExtent.height, depthFormat,
        VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        depthImage, depthImageMemory,device,physicalDevice);
    depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, device);
}

void Renderer::CreateCamera()
{
    camera = std::make_unique<Camera>(Camera::ProjectionMode::ORTHOGRATHIC, glm::vec3(0, 0, 2), glm::vec3(0, 0, 0),
        swapChainExtent.width, swapChainExtent.height);
}

VkFormat Renderer::findDepthFormat(VkPhysicalDevice physicalDevice)
{
    return findSupportedFormat( { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, physicalDevice);
}

bool Renderer::hasStencilComponent(VkFormat format)
{
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VkFormat Renderer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice physicalDevice)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }
    throw std::runtime_error("failed to find supported format!");
}

void Renderer::drawFrame(VkDevice device, VkQueue graphicsQueue, VkQueue presentQueue,
    GLFWwindow* window, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{

    if (spriteObjects != spriteObjects_new)
    {
        updateRenderables(spriteObjects_new, window, device, physicalDevice, surface);
    }



    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;

    auto result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        recreateSwapChain(window, device, physicalDevice, surface);
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image! vk error: " + std::to_string(result));
    }

    updateUniformBuffers(imageIndex, device);

    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer! vk error: " + std::to_string(result));
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;


    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
    {
        framebufferResized = false;
        recreateSwapChain(window, device, physicalDevice, surface);
        return;
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image! vk error: " + std::to_string(result));
    }

    currentFrame = (currentFrame + 1) % settings->MAX_FRAMES_IN_FLIGHT;
    spriteObjects_new.clear();
}

void Renderer::cleanup(VkDevice device)
{
    cleanupSwapChain(device);

    vkDestroySampler(device, textureSampler, nullptr);

    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

    for (size_t i = 0; i < settings->MAX_FRAMES_IN_FLIGHT; ++i)
    {
        vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, inFlightFences[i], nullptr);
    }


    for (int i = 0; i < imageViews.size(); ++i)
    {
        
        vkDestroyImageView(device, imageViews[i].second, nullptr);
        
    }
    imageViews.clear();

    for (std::tuple<std::string, VkImage, VkDeviceMemory, Vector2> image : images)
    {
        vkFreeMemory(device, std::get<2>(image), nullptr);
        vkDestroyImage(device, std::get<1>(image), nullptr);
    }
    images.clear();

    vkFreeMemory(device, depthImageMemory, nullptr);
    vkDestroyImage(device, depthImage, nullptr);
    

    

    vkDestroyCommandPool(device, commandPool, nullptr);
}

void Renderer::updateRenderables(std::vector<Renderable*> objects, GLFWwindow* window, VkDevice device,
    VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    spriteObjects = objects;
    for (auto sprite : spriteObjects)
    {
        sprite->updated1 = true;
        sprite->updated3 = true;
        sprite->updated2 = true;
    }
    // recreateSwapChain(window, device, physicalDevice, surface);
    recreateBuffers(device, physicalDevice);
}

void Renderer::renderObject(Renderable* spriteObject)
{
    spriteObjects_new.emplace_back(spriteObject);
}

std::pair<VkImage,Vector2> Renderer::createTextureImage(VkDevice device, VkPhysicalDevice physicalDevice, 
    const char* filename,  VkQueue graphicsQueue)
{

    for (std::tuple<std::string, VkImage, VkDeviceMemory, Vector2> image : images)
    {
        if (std::get<0>(image) == filename)
        {
            return std::make_pair(std::get<1>(image), std::get<3>(image));

        }
    }

    int texWidth;
    int texHeight;
    int texChannels;

    stbi_uc* pixels = stbi_load(filename, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
        // TODO : put no texture file somewhere else
        const char* noTex = "textures/NoTexture.png";
        pixels = stbi_load(noTex, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        imageSize = texWidth * texHeight * 4;
        
        std::cout << "failed to load texture file!" << std::endl;
        if (!pixels)
        {
            throw std::exception("failed to load texture image!");
        }
    }

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    Helpers::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory, device, physicalDevice);

    void* data;
    vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(device, stagingBufferMemory);

    stbi_image_free(pixels);

    VkImage image;
    VkDeviceMemory imageMemory;

    Helpers::createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        image, imageMemory, device, physicalDevice);

    transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, device, graphicsQueue);
    Helpers::copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
        device, commandPool, graphicsQueue,0,0);
    transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, device, graphicsQueue);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    std::tuple<std::string, VkImage, VkDeviceMemory,Vector2> imageTuple = std::make_tuple(filename, image, imageMemory,Vector2(static_cast<float>(texWidth),static_cast<float>(texHeight)));
    images.emplace_back(std::move(imageTuple));
    return std::make_pair(std::get<1>(images.back()), std::get<3>(images.back()));

}

std::pair<VkImage, Vector2> Renderer::CreateAtlasImage(VkDevice device, VkPhysicalDevice physicalDevice, uint32_t width, uint32_t height,
    const char* atlasname, VkQueue graphicsQueue)
{
    for (std::tuple<std::string, VkImage, VkDeviceMemory, Vector2> image : images)
    {
        if (std::get<0>(image) == atlasname)
        {
            return std::make_pair(std::get<1>(image), std::get<3>(image));
        }
    }


    VkDeviceSize imageSize = width * height * 4;
 
    

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    Helpers::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory, device, physicalDevice);



    VkImage image;
    VkDeviceMemory imageMemory;

    Helpers::createImage(width, height, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        image, imageMemory, device, physicalDevice);

    transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, device, graphicsQueue);
    transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, device, graphicsQueue);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);

    std::tuple<std::string, VkImage, VkDeviceMemory, Vector2> imageTuple = std::make_tuple(atlasname, image, imageMemory,Vector2(static_cast<float>(width),static_cast<float>(height)));
    images.emplace_back(std::move(imageTuple));
    return std::make_pair(std::get<1>(images.back()), std::get<3>(images.back()));
}

Camera* Renderer::GetCamera()
{
    return camera.get();
}

void Renderer::createTextureSampler(VkPhysicalDevice physicalDevice, VkDevice device)
{
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(physicalDevice, &properties);

    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;
    
    auto result = vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create texture sampler! vk error: " + std::to_string(result));
    }

}

void Renderer::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
    VkDevice device, VkQueue graphicsQueue)
{
    VkCommandBuffer commandBuffer = Helpers::beginSingleTimeCommands(device, commandPool);

    

    VkImageMemoryBarrier barrier{};

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    Helpers::endSingleTimeCommands(commandBuffer, device, graphicsQueue, commandPool);
}

VkImageView Renderer::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkDevice device)
{
    for (std::pair<VkImage, VkImageView> imageView : imageViews)
    {
        if (image == imageView.first)
        {
            return imageView.second;
        }
    }

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    imageViews.emplace_back(std::make_pair(image, imageView));
    return imageViews.back().second;
}

void Renderer::createSwapChain(VkPhysicalDevice PhysicalDevice, VkDevice device, VkSurfaceKHR surface, GLFWwindow* window)
{
    SwapChainSupportDetails swapChainSupport = Helpers::querySwapChainSupport(PhysicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = Helpers::findQueueFamilies(PhysicalDevice,surface);
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(),indices.presentFamily.value() };

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    auto result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create the swap chain! vkresult code: " + std::to_string(result));
    }

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

void Renderer::createImageViews(VkDevice device)
{
    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); ++i)
    {
        swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, device);
    }
}

void Renderer::createRenderPass(VkDevice device, VkPhysicalDevice physicalDevice)
{

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = findDepthFormat(physicalDevice);
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;   // sets the old values to a constant
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;  // images will be presented using the swapchain

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    VkSubpassDependency dep{};
    dep.srcSubpass = VK_SUBPASS_EXTERNAL;
    dep.dstSubpass = 0;
    dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dep.srcAccessMask = 0;
    dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dep;

    auto result = vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create render pass! vk error: " + std::to_string(result));
    }
}

void Renderer::createDescriptorSetLayout(VkDevice device)
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 1;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();



    auto result = vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout! vk error: " + std::to_string(result));
    }
}

void Renderer::createGraphicsPipeline(VkDevice device)
{
    auto vertShaderCode = Helpers::readFile("shaders/vert.spv");
    auto fragShaderCode = Helpers::readFile("shaders/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, device);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, device);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescription();


    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;   // used to set the primitive topology
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChainExtent.width);
    viewport.height = static_cast<float>(swapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = { 0,0 };
    scissor.extent = swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;  // used to set how the fragment shaders colors the polygon
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;          // Optional
    multisampling.pSampleMask = nullptr;            // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE;      // Optional

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;   // optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // optional
    colorBlending.blendConstants[1] = 0.0f; // optional
    colorBlending.blendConstants[2] = 0.0f; // optional
    colorBlending.blendConstants[3] = 0.0f; // optional

    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    auto result = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout! vkresult: " + std::to_string(result));
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;               // Optional
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;   // Optional
    pipelineInfo.basePipelineIndex = -1;                // Optional

    result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline! vk error: " + std::to_string(result));
    }


    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void Renderer::createFrameBuffers(VkDevice device)
{
    swapChainFramebuffers.resize(swapChainImageViews.size());

    
    for (size_t i = 0; i < swapChainImageViews.size(); ++i)
    {
        std::array<VkImageView, 2> attachments =
        {
            swapChainImageViews[i],
            depthImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        auto result = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer! vk error: " + std::to_string(result));
        }

    }
}

void Renderer::createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    QueueFamilyIndices queueFamilyIndices = Helpers::findQueueFamilies(physicalDevice, surface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = 0;

    auto result = vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool! vk error: " + std::to_string(result));
    }
}

void Renderer::createUniformBuffers(VkDevice device, VkPhysicalDevice physicalDevice)
{
    auto dynamic_size = spriteObjects.size();
    if (dynamic_size <= 0)
    {
        dynamic_size = 1;
    }
    VkDeviceSize bufferSize = sizeof(UniformBufferObject) * dynamic_size;
    uniformBuffers.resize(swapChainImages.size());
    uniformBuffersMemory.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); ++i)
    {
        Helpers::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            uniformBuffers[i], uniformBuffersMemory[i], device, physicalDevice);
    }
}

void Renderer::createDescriptorPool(VkDevice device)
{

    size_t size = spriteObjects.size();
    if (size <= 0)
    {
        size = 1;
    }

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size()) * static_cast<uint32_t>(size);

    auto result = vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool! vk error: " + std::to_string(result));
    }
}

void Renderer::createDescriptorSets(VkDevice device, std::vector<VkDescriptorSet>& _descriptorSets, VkImageView textureImageView)
{
    for (std::pair<VkImageView, std::vector<VkDescriptorSet>> descriptorSet : descriptorSets)
    {
        if (textureImageView == descriptorSet.first)
        {
            _descriptorSets.resize(swapChainImages.size());
            _descriptorSets[0] = (descriptorSet.second[0]);
            _descriptorSets[1] = (descriptorSet.second[1]);
            _descriptorSets[2] = (descriptorSet.second[2]);
            return;
        }
    }

    std::vector<VkDescriptorSet> descriptorSet;

    std::vector<VkDescriptorSetLayout> layouts(swapChainImages.size(), descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();

    descriptorSet.resize(swapChainImages.size());

    auto result = vkAllocateDescriptorSets(device, &allocInfo, descriptorSet.data());
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets! vk error: " + std::to_string(result));
    }

    _descriptorSets.resize(swapChainImages.size());
    for (size_t i = 0; i < swapChainImages.size(); ++i)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = textureImageView;
        imageInfo.sampler = textureSampler;


        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSet[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSet[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        
        _descriptorSets[i] = (descriptorSet[i]);
        
        
    }
    descriptorSets.emplace_back(std::make_pair(textureImageView, std::move(descriptorSet)));
}

void Renderer::createCommandBuffers(VkDevice device)
{
    commandBuffers.resize(swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    auto result = vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data());
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command buffers! vk error: " + std::to_string(result));
    }

    for (size_t i = 0; i < commandBuffers.size(); ++i)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        result = vkBeginCommandBuffer(commandBuffers[i], &beginInfo);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer! vk error: " + std::to_string(result));
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[i];
        renderPassInfo.renderArea.offset = { 0,0 };
        renderPassInfo.renderArea.extent = swapChainExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { settings->backgroundColorRed,settings->backgroundColorGreen,settings->backgroundColorBlue,1.f };
        clearValues[1].depthStencil = { 1.0f,0 };
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);


        for (size_t x = 0; x < spriteObjects.size(); ++x)
        {
            std::vector<VkDescriptorSet> descriptorSet;
            for (std::pair<VkImageView, std::vector<VkDescriptorSet>> descriptor : descriptorSets)
            {
                if (spriteObjects[x]->GetTextureImageView() == descriptor.first)
                {
                    descriptorSet = descriptor.second;
                }
            }

        
            VkBuffer vertexBuffers[] = { spriteObjects[x]->GetVertexBuffer() };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(commandBuffers[i], spriteObjects[x]->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);
            uint32_t dynamicOffset = static_cast<uint32_t>(x * sizeof(UniformBufferObject));
            vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet[i], 1, &dynamicOffset);


            vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(spriteObjects[x]->GetIndices().size()), 1, 0, 0, 0);
        }
        vkCmdEndRenderPass(commandBuffers[i]);

        result = vkEndCommandBuffer(commandBuffers[i]);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer! vk error: " + std::to_string(result));
        }
    }
}

void Renderer::createSyncObjects(VkDevice device)
{
    imageAvailableSemaphores.resize(settings->MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(settings->MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(settings->MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < settings->MAX_FRAMES_IN_FLIGHT; ++i)
    {
        auto result_image = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
        auto result_render = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]);
        auto result_fence = vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]);

        if ((result_image || result_render) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create semaphores! vk error image: " + std::to_string(result_image) + " vk error render: " + std::to_string(result_render));
        }
        if (result_fence != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create fence! vk error: " + std::to_string(result_fence));
        }
    }
}

void Renderer::updateUniformBuffers(uint32_t currentImage, VkDevice device)
{
    UniformBufferObject ubo{};
    for (size_t i = 0; i < spriteObjects.size(); ++i)
    {
        if (currentImage == 0 && spriteObjects[i]->updated1 || currentImage == 1 && spriteObjects[i]->updated2 || currentImage == 2 && spriteObjects[i]->updated3 ||
            currentImage == 0 && camera->updated1 || currentImage == 1 && camera->updated2 || currentImage == 2 && camera->updated3)
        {

            ubo.model = spriteObjects[i]->GetModelMatrix();
            ubo.view = camera->GetView();
            ubo.proj = camera->GetProjection();

            

            void* data;
            vkMapMemory(device, uniformBuffersMemory[currentImage], sizeof(UniformBufferObject) * i, sizeof(UniformBufferObject), 0, &data);
            memcpy(data, &ubo, sizeof(UniformBufferObject));
            vkUnmapMemory(device, uniformBuffersMemory[currentImage]);

            if (currentImage == 0)
            {
                spriteObjects[i]->updated1 = false;
            }
            else if (currentImage == 1)
            {
                spriteObjects[i]->updated2 = false;
            }
            else if (currentImage == 2)
            {
                spriteObjects[i]->updated3 = false;
            }
            
        }
    }
    if (currentImage == 0)
    {
        camera->updated1 = false;
    }
    else if (currentImage == 1)
    {
        camera->updated2 = false;
    }
    else if (currentImage == 2)
    {
        camera->updated3 = false;
    }

}

void Renderer::recreateSwapChain(GLFWwindow* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(device);

    cleanupSwapChain(device);

    createSwapChain(physicalDevice, device, surface, window);
    createImageViews(device);
    createRenderPass(device, physicalDevice);
    createGraphicsPipeline(device);
    createFrameBuffers(device);
    createUniformBuffers(device, physicalDevice);
    createDescriptorPool(device);
    for (auto& sprite : spriteObjects)
    {
        createDescriptorSets(device, sprite->descriptorSets, sprite->GetTextureImageView());
    }
    createCommandBuffers(device);
}

void Renderer::recreateBuffers(VkDevice device, VkPhysicalDevice physicalDevice)
{
    vkDeviceWaitIdle(device);
    for (size_t i = 0; i < swapChainImages.size(); ++i)
    {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }
    vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    descriptorSets.clear();

    createUniformBuffers(device, physicalDevice);
    createDescriptorPool(device);
    for (auto& sprite : spriteObjects)
    {
        createDescriptorSets(device,sprite->descriptorSets,sprite->GetTextureImageView());
    }
    createCommandBuffers(device);
}

void Renderer::cleanupSwapChain(VkDevice device)
{
    for (size_t i = 0; i < swapChainImages.size(); ++i)
    {
        vkDestroyBuffer(device, uniformBuffers[i], nullptr);
        vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
    }

   

    vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    vkDestroyPipeline(device, graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
    vkDestroyRenderPass(device, renderPass, nullptr);
    for (auto framebuffer : swapChainFramebuffers)
    {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    
    for (auto imageView : swapChainImageViews)
    {
        vkDestroyImageView(device, imageView, nullptr);
        for (int i = 0; i < imageViews.size(); ++i)
        {
            if (imageView == imageViews[i].second)
            {
                auto it = imageViews.begin() + i;
                imageViews.erase(it);
            }
        }
        imageView = VK_NULL_HANDLE;
    }
    
    vkDestroySwapchainKHR(device, swapChain, nullptr);
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);

    

    descriptorSets.clear();
}

VkShaderModule Renderer::createShaderModule(const std::vector<char>& code, VkDevice device)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    auto result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module! vkresult: " + std::to_string(result));
    }

    return shaderModule;
}

VkSurfaceFormatKHR Renderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR Renderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresetModes)
{
    for (const auto& availablePresetMode : availablePresetModes)
    {
        if (availablePresetMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresetMode;
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Renderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{
    if (capabilities.currentExtent.width != UINT32_MAX)
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width = 0;
        int height = 0;

        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}
