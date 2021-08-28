#include "../3rdParty/json/json.hpp"
#include "../jam/Configuration.h"
#include "../jam/BitmapFont.h"
#include "../jam/Shared.h"

namespace jam
{
    BitmapFont::BitmapFont()
    {
        this->runes.clear();
        this->texture = nullptr;
    }

    BitmapFont::~BitmapFont()
    {
        this->runes.clear();
        this->texture = nullptr;
    }

    void BitmapFont::Construct(std::string filePath)
    {
        std::string texturePath = "";
        this->runes.clear();
        nlohmann::json ret = jam::Configuration::LoadJsonFile(filePath);
        texturePath = "assets/image/" + ret["image"].get<std::string>(); // p.replace_filename(ret["image"].get<std::string>()).string();
        if (!jam::backEnd->ResourceManager()->HasImage(texturePath))
        {
            jam::backEnd->ResourceManager()->PreloadImage(texturePath);
        }
        this->texture = jam::backEnd->ResourceManager()->GetImage(texturePath);
        for (auto& item : ret["runes"])
        {
            int x, y, w, h;
            char rune;
            rune = item["rune"].get<std::string>()[0];
            x = item["x"].get<int>();
            y = item["y"].get<int>();
            w = item["width"].get<int>();
            h = item["height"].get<int>();
            this->runes[rune] = Rect{ x, y, w, h };
        }
    }

    void BitmapFont::DrawText(jam::IRenderer* render, std::string message, int x, int y, jam::rgb color)
    {
        int currentX = x;
        int lineW, lineH;
        this->MeasureText(message, &lineW, &lineH);
        for (char c : message)
        {
            std::unordered_map<char, jam::Rect>::const_iterator search = this->runes.find(c);
            if (search != this->runes.end())
            {
                render->DrawSubImage(this->texture, currentX, y - lineH, (*search).second.x, (*search).second.y, (*search).second.w, (*search).second.h);
                currentX += (*search).second.w;
            }
        }
    }

    void BitmapFont::MeasureText(std::string message, int* width, int* height)
    {
        int totalWidth = 0;
        int maxHeight = 0;
        for (char c : message)
        {
            std::unordered_map<char, jam::Rect>::const_iterator search = this->runes.find(c);
            if (search != this->runes.end())
            {
                totalWidth += (*search).second.w;
                if (maxHeight < (*search).second.h)
                    maxHeight = (*search).second.h;
            }
        }
        *width = totalWidth;
        *height = maxHeight;
    }

}