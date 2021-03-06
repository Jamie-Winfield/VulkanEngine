#include "VulkanEngineApplication.h"
#include "Mouse.h"

VulkanEngineApplication::VulkanEngineApplication(std::unique_ptr<Settings> _settings)
{
    settings = std::move(_settings);
}

void VulkanEngineApplication::run()
{
    engine = std::make_unique<Engine>();
    
    initWindow();
    engine->initVulkan(settings.get(), window);
    game = std::make_unique<Game>();
    GameHelper::Init(engine.get());
    mainLoop();
    game->End();
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void VulkanEngineApplication::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    KeyEvent keyEvent{ key,scancode,action,mods };
    auto app = reinterpret_cast<VulkanEngineApplication*>(glfwGetWindowUserPointer(window));
    app->keyEvents.emplace_back(keyEvent);
}

void VulkanEngineApplication::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    KeyEvent keyEvent{ button,0,action,mods };
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
    game->Start();
    timeBetweenFrames = 0.0001;
    while (!glfwWindowShouldClose(window))
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        glfwPollEvents();
        Mouse mouse;
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mouse.x_pos = static_cast<float>(xpos);
        mouse.y_pos = static_cast<float>(ypos);
        for (auto& keyEvent : keyEvents)
        {
            game->KeyHandler(keyEvent, mouse);
        }
        keyEvents.clear();
        GameHelper::SetGameTime(timeBetweenFrames);
        game->Update(timeBetweenFrames);
        game->Render(engine->getRenderer());
        engine->getRenderer()->drawFrame(engine->getDevice(), engine->getGraphicsQueue(), engine->getPresentQueue(),
            window, engine->getPhysicalDevice(), engine->getSurface());
        auto finishTime = std::chrono::high_resolution_clock::now();
        timeBetweenFrames = std::chrono::duration<float, std::chrono::seconds::period>(finishTime - startTime).count();
        if (engine->GetCollisionSystem() != nullptr)
        {
            engine->GetCollisionSystem()->UpdateCollsions();
        }
        if (engine->GetFrameCounterEnable())
        {
            frameCounter(timeBetweenFrames);
        } 
       
        else
        {
            glfwSetWindowTitle(window, settings->window_name.c_str());
        }
    }

    vkDeviceWaitIdle(engine->getDevice());
}

void VulkanEngineApplication::frameCounter(float time)
{
    frame_count++;
    time_count += time;
    if (time_count >= 1)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        ypos = settings->W_HEIGHT - ypos;
        std::stringstream ss;
        ss << "FPS: " << "  " << frame_count << " MouseX: " << xpos << " MouseY: " << ypos ;

        glfwSetWindowTitle(window, ss.str().c_str());
        frame_count = 0;
        time_count = 0;
    }
}
