#include "VulkanEngineApplication.h"


int main()
{

    auto settings = std::make_unique<Settings>();
    settings->W_WIDTH = 800;
    settings->W_HEIGHT = 800;
    settings->window_name = "VulkanGame";
    settings->backgroundColorRed = 0;
    settings->backgroundColorBlue = 1;
    settings->backgroundColorGreen = 0;
    VulkanEngineApplication app(std::move(settings));

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
