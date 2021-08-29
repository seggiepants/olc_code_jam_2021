#include "TileSet.h"
#include "Shared.h"

namespace jam
{
	TileSet::TileSet()
	{

	}

	TileSet::~TileSet()
	{

	}

	void TileSet::Construct(std::string fileName)
	{
		tinyxml2::XMLDocument doc;
		doc.LoadFile(fileName.c_str());
		tinyxml2::XMLElement* tileSetElement = doc.FirstChildElement("tileset");
		this->name = tileSetElement->FindAttribute("name")->Value();
		this->tileWidth = tileSetElement->FindAttribute("tilewidth")->IntValue();
		this->tileHeight = tileSetElement->FindAttribute("tileheight")->IntValue();
		this->tileCount = tileSetElement->FindAttribute("tilecount")->IntValue();
		this->columns = tileSetElement->FindAttribute("columns")->IntValue();
		tinyxml2::XMLElement* imageElement = tileSetElement->FirstChildElement("image");
		this->source = imageElement->FindAttribute("source")->Value();
		this->width = imageElement->FindAttribute("width")->IntValue();
		this->height = imageElement->FindAttribute("height")->IntValue();

		if (this->source.length() > 0)
		{
			std::string fileName = jam::IMAGE_PATH + this->source;
			if (!jam::backEnd->ResourceManager()->HasImage(fileName))
			{
				jam::backEnd->ResourceManager()->PreloadImage(fileName);
			}
		}
	}

	std::string TileSet::ImageKey()
	{
		return jam::IMAGE_PATH + this->source;
	}
}