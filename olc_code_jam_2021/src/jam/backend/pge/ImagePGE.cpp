#include <iostream>
#include "ImagePGE.h"
#include "RendererPGE.h"

namespace jam
{
    ImagePGE::ImagePGE()
    {
        this->width = this->height = this->bpp = 0;
        this->sprite = nullptr;
        //this->decal = nullptr;
    }

    ImagePGE::~ImagePGE()
    {
        /*
        if (this->decal != nullptr)
        {
            delete this->decal;
            this->decal = nullptr;
        }
        */

        if (this->sprite != nullptr)
        {
            delete this->sprite;
            this->sprite = nullptr;
        }
    }

    bool ImagePGE::Construct(std::string fileName)
    {
        //this->decal = nullptr;
        this->sprite = this->LoadSprite(fileName);
        /*
        if (this->sprite != nullptr)
        {
            this->decal = new olc::Decal(this->sprite);
        }
        */
        return this->sprite != nullptr;
    }

    int ImagePGE::GetWidth()
    {
        if (this->sprite == nullptr)
        {
            return 0;
        }
        else
        {
            return this->sprite->width;
        }
    }

    int ImagePGE::GetHeight()
    {
        if (this->sprite == nullptr)
        {
            return 0;
        }
        else
        {
            return this->sprite->height;
        }

    }

    olc::Sprite* ImagePGE::GetHandle()
    {
        return this->sprite;
    }

    olc::Sprite* ImagePGE::LoadSprite(std::string fileName)
    {
        // Load image at specified path
        olc::Sprite* sprite = new olc::Sprite();
        olc::rcode ret = sprite->LoadFromFile(fileName);
        if (ret != olc::rcode::OK)
        {
            std::cerr << "Unable to load image \"" << fileName << "\"" << std::endl;
            sprite = nullptr;
        }
        return sprite;
    }
}
