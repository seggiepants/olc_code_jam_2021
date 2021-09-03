#pragma once
#ifndef __TILE_MAP_H__
#define __TILE_MAP_H__

#include <string>
#include "Configuration.h"
#include "IRenderer.h"
#include "TileMapLayer.h"
#include "TileSet.h"

namespace jam
{
	class TileMap
	{
	public:
		TileMap();
		~TileMap();
		void Construct(std::string fileName);
		void Draw(IRenderer* render, int offsetX, int offsetY, int top, int left, int width, int height);
		int GetTile(int layer, int x, int y);
		int GetWidth() { return this->width * this->tileWidth; };
		int GetHeight() { return this->height * this->tileHeight; };
		TileMapLayer* GetLayer(int layerNum) { return this->layers[layerNum]; };
		int GetLayerCount() { return this->layers.size(); };
		int GetTileWidth() { return this->tileWidth; };
		int GetTileHeight() { return this->tileHeight; };
	private:
		int tileWidth;
		int tileHeight;
		int width;
		int height;
		std::vector<TileMapLayer*> layers;
		std::vector<TileSet*> tileSets;
	};
}
#endif