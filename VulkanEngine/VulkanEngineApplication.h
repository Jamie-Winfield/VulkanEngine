#pragma once
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
#include "GameHelper.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

#include "keyEvent.h"
class VulkanEngineApplication
{

private:
    GLFWwindow* window;
    std::unique_ptr<Settings> settings;
    std::unique_ptr<Game> game;

    float timeBetweenFrames = 0;

    float frame_count = 0;
    float time_count = 0;
    std::vector<KeyEvent> keyEvents;




private:
    std::unique_ptr<Engine> engine;
    std::unique_ptr<GameHelper> game_helper;


public:
    void run();

private:
    void cleanup();
    void initWindow();
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
    void mainLoop();
    void frameCounter(float time);
};

