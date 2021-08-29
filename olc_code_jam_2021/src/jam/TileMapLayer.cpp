#include "TileMapLayer.h"

namespace jam
{
	TileMapLayer::TileMapLayer()
	{

	}

	TileMapLayer::~TileMapLayer()
	{
		this->data.clear();
	}

	void TileMapLayer::Construct(nlohmann::json node)
	{
		std::vector<int> temp;
		temp = node["data"].get<std::vector<int>>();
		this->data.clear();
		for (std::vector<int>::iterator current = temp.begin(); current != temp.end(); current++)
		{
			this->data.push_back(*current);
		}
		this->height = node["height"].get<int>();
		this->id = node["id"].get<int>();
		this->name = node["name"];
		this->opacity = node["opacity"].get<int>();
		this->type = node["type"];
		this->visible = node["visible"].get<bool>();
		this->width = node["width"].get<int>();
		this->x = node["x"].get<int>();
		this->y = node["y"].get<int>();
	}

	int TileMapLayer::GetTileID(int x, int y)
	{
		if (x >= 0 && x <= this->width && y >= 0 && y < this->height)
		{
			return this->data[(y * this->width) + x] - 1;
		}
		else
		{
			return 0;
		}
	}

	void TileMapLayer::GetTilePos(int tileID, TileSet* tileSet, int* x, int* y)
	{
		int tileX, tileY, tileW, tileH;
		tileW = (tileSet->GetWidth() - (tileSet->GetWidth() % tileSet->GetTileWidth())) / tileSet->GetTileWidth();
		tileX = tileID % tileW;
		tileY = (tileID - tileX) / tileW;
		*x = tileX * tileSet->GetTileWidth();
		*y = tileY * tileSet->GetTileHeight();
	}

}