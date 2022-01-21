#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <memory>
#include <string>

#include "settings.hpp"
#include "Engine.h"
#include "Game.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

//TODO TEXTURE MAPPING



static std::vector<int> keyEvents;

class HelloTriangleApplication
{
private:
    GLFWwindow* window;
    std::unique_ptr<Settings> settings;
    std::unique_ptr<Game> game;

    float timeBetweenFrames = 0;


private:
    std::unique_ptr<Engine> engine;


public:
    void run()
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

private:

    void cleanup()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    void initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(settings->W_WIDTH, settings->W_HEIGHT, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
        glfwSetKeyCallback(window, key_callback);
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (std::find(keyEvents.begin(),keyEvents.end(),key) == keyEvents.end())
        {
            keyEvents.emplace_back(key);
        }
    }

    static void framebufferResizeCallback(GLFWwindow* window,int width,int height)
    {
        auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        app->engine->getRenderer()->framebufferResized = true;
    }



    void mainLoop()
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
            game->render();
            engine->getRenderer()->drawFrame(engine->getDevice(),engine->getGraphicsQueue(),engine->getPresentQueue(),window,engine->getPhysicalDevice(),engine->getSurface());
            //updateRotations(timeBetweenFrames);
            auto finishTime = std::chrono::high_resolution_clock::now();
            timeBetweenFrames = std::chrono::duration<float, std::chrono::seconds::period>(finishTime - startTime).count();

            
        }

        vkDeviceWaitIdle(engine->getDevice());
    }




};



int main()
{
    HelloTriangleApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}