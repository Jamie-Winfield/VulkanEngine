#include "VulkanEngineApplication.h"


int main()
{

    auto settings = std::make_unique<Settings>();
    settings->W_WIDTH = 1920;
    settings->W_HEIGHT = 1080;
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
