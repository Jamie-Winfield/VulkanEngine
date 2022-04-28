#include "TextureAtlas.h"
#include "helpers.cpp"


TextureAtlas::TextureAtlas(uint32_t width, uint32_t height, Engine* _engine, const char* _atlasname)
{
	size = UVector2(width, height);
	engine = _engine;
	atlasname = _atlasname;
    Area area;
    lastArea = area;
    rowStartArea = lastArea;
	CreateAtlas();
}

bool TextureAtlas::AddImage(const char* filename)
{
    int texWidth;
    int texHeight;
    int texChannels;

    unsigned char* pixels = engine->getRenderer()->GetPixels(filename, texWidth, texHeight, texChannels);
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
        // TODO : put no texture file somewhere else
        filename = "textures/NoTexture.png";
        pixels = engine->getRenderer()->GetPixels(filename, texWidth, texHeight, texChannels);;
        

        std::cout << "failed to load texture file!" << std::endl;
        if (!pixels)
        {
            throw std::exception("failed to load texture image!");
        }
    }

    imageSize = texWidth * texHeight * 4;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    Area area;


    if (lastArea.posMax.x + texWidth < size.x)
    {
        if (lastArea.position.y + texHeight < size.y)
        {
            area = Area(Vector2(lastArea.posMax.x, lastArea.position.y),UVector2(texWidth,texHeight));
        }
        else
        {
            throw std::exception("Not enough space in texture atlas");
            // not enough space
        }
    }
    else
    {
        if (rowStartArea.posMax.y + texHeight < size.y)
        {
            area = Area(Vector2(rowStartArea.position.x, rowStartArea.posMax.y), UVector2(texWidth, texHeight));
            rowStartArea = area;
        }
        else
        {
            throw std::exception("Not enough space in texture atlas");
            // not enough space
        }
    }

    lastArea = area;
    if (area.posMax.y > rowStartArea.posMax.y)
    {
        rowStartArea.posMax.y = area.posMax.y;
    }

    Helpers::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory, engine->getDevice(), engine->getPhysicalDevice());

    void* data;
    vkMapMemory(engine->getDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(engine->getDevice(), stagingBufferMemory);

    engine->getRenderer()->FreePixels(pixels);

    engine->getRenderer()->transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, engine->getDevice(), engine->getGraphicsQueue());

    Helpers::copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
        engine->getDevice(), engine->getRenderer()->getCommandPool(), engine->getGraphicsQueue(), static_cast<uint32_t>(area.position.x), static_cast<uint32_t>(area.position.y));

    engine->getRenderer()->transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, engine->getDevice(), engine->getGraphicsQueue());

    vkDestroyBuffer(engine->getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(engine->getDevice(), stagingBufferMemory, nullptr);

    usedAreas.emplace_back(std::make_pair(filename, area));

    return true;
}

bool TextureAtlas::AddText(const char* name, unsigned char* buffer, int width, int height, int y_offset)
{
    int texWidth = width;
    int texHeight = height;

    unsigned char* pixels = buffer;
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
        throw std::exception("failed to load pixels from buffer");
        
    }

    

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    Area area;

    
   

    if (lastArea.posMax.x + texWidth  < size.x)
    {
        if (rowStartArea.position.y + texHeight + y_offset < size.y)
        {
            area = Area(Vector2(lastArea.posMax.x, rowStartArea.position.y + y_offset), UVector2(texWidth, texHeight));
        }
        else
        {
            throw std::exception("Not enough space in texture atlas");
            // not enough space
        }
    }
    else
    {
        if (rowStartArea.posMax.y + texHeight+ y_offset < size.y)
        {
            area = Area(Vector2(rowStartArea.position.x, rowStartArea.posMax.y + y_offset), UVector2(texWidth, texHeight));
            rowStartArea = area;
        }
        else
        {
            throw std::exception("Not enough space in texture atlas");
            // not enough space
        }
    }

    lastArea = area;
    if (area.posMax.y > rowStartArea.posMax.y)
    {
        rowStartArea.posMax.y = area.posMax.y;
    }

    Helpers::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory, engine->getDevice(), engine->getPhysicalDevice());

    void* data;
    vkMapMemory(engine->getDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(engine->getDevice(), stagingBufferMemory);

    engine->getRenderer()->FreePixels(pixels);

    engine->getRenderer()->transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, engine->getDevice(), engine->getGraphicsQueue());

    Helpers::copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
        engine->getDevice(), engine->getRenderer()->getCommandPool(), engine->getGraphicsQueue(), static_cast<uint32_t>(area.position.x), static_cast<uint32_t>(area.position.y));

    engine->getRenderer()->transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, engine->getDevice(), engine->getGraphicsQueue());

    vkDestroyBuffer(engine->getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(engine->getDevice(), stagingBufferMemory, nullptr);

    usedAreas.emplace_back(std::make_pair(name, area));

    return true;
}

void TextureAtlas::AddEmptySpace(int width, int height)
{
    Area area;
    if (lastArea.posMax.x + width < size.x)
    {
        if (rowStartArea.position.y + height < size.y)
        {
            area = Area(Vector2(lastArea.posMax.x, rowStartArea.position.y), UVector2(width, height));
        }
        else
        {
            throw std::exception("Not enough space in texture atlas");
            // not enough space
        }
    }
    else
    {
        if (rowStartArea.posMax.y + width < size.y)
        {
            area = Area(Vector2(rowStartArea.position.x, rowStartArea.posMax.y), UVector2(width, height));
            rowStartArea = area;
        }
        else
        {
            throw std::exception("Not enough space in texture atlas");
            // not enough space
        }
    }
    lastArea = area;
    usedAreas.emplace_back(std::make_pair("EmptySpace", area));
}

void TextureAtlas::AddEmptySpaceNewLine(int width, int height)
{
    Area area;
    if (rowStartArea.posMax.y + height < size.y)
    {
        area = Area(Vector2(rowStartArea.position.x, rowStartArea.posMax.y), UVector2(width, height));
        rowStartArea = area;
    }
    else
    {
        throw std::exception("Not enough space in texture atlas");
        // not enough space
    }
    lastArea = area;
    usedAreas.emplace_back(std::make_pair("EmptySpace", area));
}

void TextureAtlas::AddImageNewLine(const char* name, unsigned char* buffer, int width, int height)
{
    int texWidth = width;
    int texHeight = height;

    unsigned char* pixels = buffer;
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
        throw std::exception("failed to load pixels from buffer");

    }



    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    Area area;

    if (rowStartArea.posMax.y + texHeight < size.y)
    {
        area = Area(Vector2(rowStartArea.position.x, rowStartArea.posMax.y), UVector2(texWidth, texHeight));
        rowStartArea = area;
    }
    else
    {
        throw std::exception("Not enough space in texture atlas");
        // not enough space
    }
    lastArea = area;
    if (area.posMax.y > rowStartArea.posMax.y)
    {
        rowStartArea.posMax.y = area.posMax.y;
    }

    Helpers::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory, engine->getDevice(), engine->getPhysicalDevice());

    void* data;
    vkMapMemory(engine->getDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(engine->getDevice(), stagingBufferMemory);

    engine->getRenderer()->FreePixels(pixels);

    engine->getRenderer()->transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, engine->getDevice(), engine->getGraphicsQueue());

    Helpers::copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
        engine->getDevice(), engine->getRenderer()->getCommandPool(), engine->getGraphicsQueue(), static_cast<uint32_t>(area.position.x), static_cast<uint32_t>(area.position.y));

    engine->getRenderer()->transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, engine->getDevice(), engine->getGraphicsQueue());

    vkDestroyBuffer(engine->getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(engine->getDevice(), stagingBufferMemory, nullptr);

    usedAreas.emplace_back(std::make_pair(name, area));
}

void TextureAtlas::AddTextNewLine(const char* name, unsigned char* buffer, int width, int height, int y_offset)
{
    int texWidth = width;
    int texHeight = height;


    unsigned char* pixels = buffer;
    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if (!pixels)
    {
        throw std::exception("failed to load pixels from buffer");

    }



    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    Area area;

    if (rowStartArea.posMax.y + texHeight + y_offset < size.y)
    {
        area = Area(Vector2(rowStartArea.position.x, rowStartArea.posMax.y + y_offset), UVector2(texWidth, texHeight));
        auto area2 = Area(Vector2(rowStartArea.position.x, rowStartArea.posMax.y), UVector2(texWidth, texHeight));
        rowStartArea = area2;
    }
    else
    {
        throw std::exception("Not enough space in texture atlas");
        // not enough space
    }
    lastArea = area;
    if (area.posMax.y > rowStartArea.posMax.y)
    {
        rowStartArea.posMax.y = area.posMax.y;
    }

    Helpers::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer, stagingBufferMemory, engine->getDevice(), engine->getPhysicalDevice());

    void* data;
    vkMapMemory(engine->getDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(engine->getDevice(), stagingBufferMemory);

    engine->getRenderer()->FreePixels(pixels);

    engine->getRenderer()->transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, engine->getDevice(), engine->getGraphicsQueue());

    Helpers::copyBufferToImage(stagingBuffer, image, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight),
        engine->getDevice(), engine->getRenderer()->getCommandPool(), engine->getGraphicsQueue(), static_cast<uint32_t>(area.position.x), static_cast<uint32_t>(area.position.y));

    engine->getRenderer()->transitionImageLayout(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, engine->getDevice(), engine->getGraphicsQueue());

    vkDestroyBuffer(engine->getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(engine->getDevice(), stagingBufferMemory, nullptr);

    usedAreas.emplace_back(std::make_pair(name, area));
}



Vector2x2 TextureAtlas::GetUV(const char* filename)
{
    for (auto &area : usedAreas)
    {
        if (area.first == filename)
        {
            Vector2 x = Vector2(area.second.position.x / static_cast<float>(size.x), area.second.position.y / static_cast<float>(size.y));
            Vector2 y = Vector2(area.second.posMax.x / static_cast<float>(size.x), area.second.posMax.y / static_cast<float>(size.y));

            return Vector2x2(x, y);
        }
    }
    throw std::exception("file not in atlas!");
}

void TextureAtlas::CreateAtlas()
{
	image = engine->getRenderer()->CreateAtlasImage(engine->getDevice(), engine->getPhysicalDevice(), size.x, size.y,
		atlasname, engine->getGraphicsQueue()).first;
	imageView = engine->getRenderer()->createImageView(image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, engine->getDevice());

}
