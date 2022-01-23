#include "VulkanEngineApplication.h"

void VulkanEngineApplication::run()
{
    engine = std::make_unique<Engine>();
    settings = std::make_unique<Settings>();
    game = std::make_unique<Game>(engine.get());
    initWindow();
    engine->initVulkan(settings.get(), window);
    mainLoop();
    game->end(engine->getDevice());
    engine->cleanup();
    cleanup();
}

void VulkanEngineApplication::cleanup()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void VulkanEngineApplication::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(settings->W_WIDTH, settings->W_HEIGHT, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, key_callback);
}

void VulkanEngineApplication::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    KeyEvent keyEvent{ key,scancode,action,mods };
    auto app = reinterpret_cast<VulkanEngineApplication*>(glfwGetWindowUserPointer(window));
    app->keyEvents.emplace_back(keyEvent);
}

void VulkanEngineApplication::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
    auto app = reinterpret_cast<VulkanEngineApplication*>(glfwGetWindowUserPointer(window));
    app->engine->getRenderer()->framebufferResized = true;
}

void VulkanEngineApplication::mainLoop()
{
    game->start();
    while (!glfwWindowShouldClose(window))
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        glfwPollEvents();
        for (auto& keyEvent : keyEvents)
        {
            game->keyHandler(keyEvent);
        }
        keyEvents.clear();
        game->update(timeBetweenFrames);
        game->render(engine->getRenderer());
        engine->getRenderer()->drawFrame(engine->getDevice(), engine->getGraphicsQueue(), engine->getPresentQueue(),
            window, engine->getPhysicalDevice(), engine->getSurface());
        auto finishTime = std::chrono::high_resolution_clock::now();
        timeBetweenFrames = std::chrono::duration<float, std::chrono::seconds::period>(finishTime - startTime).count();
        frameCounter(timeBetweenFrames);

    }

    vkDeviceWaitIdle(engine->getDevice());
}

void VulkanEngineApplication::frameCounter(float time)
{
    frame_count++;
    time_count += time;
    if (time_count >= 1)
    {

        std::stringstream ss;
        ss << "FPS" << "  " << frame_count;

        glfwSetWindowTitle(window, ss.str().c_str());
        frame_count = 0;
        time_count = 0;
    }
}
