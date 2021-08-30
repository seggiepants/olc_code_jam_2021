#include "TileMap.h"

namespace jam
{
	TileMap::TileMap()
	{

	}
	
	TileMap::~TileMap()
	{
		for (TileMapLayer* layer : this->layers)
		{
			delete layer;
		}
		this->layers.clear();

		for (TileSet* tileSet : this->tileSets)
		{
			delete tileSet;
		}
		this->tileSets.clear();
	}

	void TileMap::Construct(std::string fileName)
	{
		std::string tmxSource = "";
		nlohmann::json data = Configuration::LoadJsonFile(fileName);
		if (data != nullptr)
		{
			this->tileWidth = data["tilewidth"].get<int>();
			this->tileHeight = data["tileheight"].get<int>();
			this->width = data["width"].get<int>();
			this->height = data["height"].get<int>();
			this->layers.clear();
			for (auto& layer : data["layers"])
			{
				TileMapLayer* newLayer = new TileMapLayer();
				newLayer->Construct(layer);
				this->layers.push_back(newLayer);
			}

			for (auto& tileSet : data["tilesets"])
			{
				TileSet* newTileSet = new TileSet();
				std::string fileName;
				fileName = jam::MAP_PATH + tileSet["source"].get<std::string>();
				newTileSet->Construct(fileName);
				this->tileSets.push_back(newTileSet);
			}
		}
	}

	void TileMap::Draw(IRenderer* render, int offsetX, int offsetY, int top, int left, int width, int height)
	{
		int i, j; // Tile Index;
		i = j = 0;
		int i0, j0; // x,y location of the data offset.
		int dx, dy;
		dx = offsetX % this->tileWidth;
		dy = offsetY % this->tileHeight;
		i0 = (offsetX - dx) / this->tileWidth;
		j0 = (offsetY - dy) / this->tileHeight;
		jam::IImage* image = jam::backEnd->ResourceManager()->GetImage(this->tileSets[0]->ImageKey());		
		
		for (int layerNum = 0; layerNum < this->layers.size(); layerNum++)
		{
			j = 0;
  			for (int y = top; y <= top + height && y < this->height * this->tileHeight; y += this->tileHeight)
			{
				i = 0;
				for (int x = left; x <= left + width && x < this->width * this->tileWidth; x += this->tileWidth)
				{
					int tileID = this->GetTile(layerNum, i + i0, j + j0);
					if (tileID > 0)
					{
						int tx, ty;
						tx = ty = 0;
						this->layers[layerNum]->GetTilePos(tileID, this->tileSets[0], &tx, &ty);
						render->DrawSubImage(image, x - dx, y - dy, tx, ty, this->tileWidth, this->tileHeight);
					}
					i++;
				}
				j++;
			}
		}
	}

	int TileMap::GetTile(int layer, int x, int y)
	{
		return this->layers[layer]->GetTileID(x, y);
	}
}