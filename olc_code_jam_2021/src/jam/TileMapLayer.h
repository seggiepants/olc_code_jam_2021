#pragma once
#ifndef __TILE_MAP_LAYER_H__
#define __TILE_MAP_LAYER_H__

#include <string>
#include <vector>
#include "../3rdParty/json/json.hpp"
#include "TileSet.h"

namespace jam
{
	class TileMapLayer
	{
	public:
		TileMapLayer();
		~TileMapLayer();
		void Construct(nlohmann::json node);
		int GetWidth() { return this->width; };
		int GetHeight() { return this->height; };
		int GetTileID(int x, int y);
		void GetTilePos(int tileID, TileSet* tileSet, int* x, int* y);
		std::string GetName() { return this->name; };
	protected:
		std::vector<int> data;
		int height;
		int id;
		std::string name;
		int opacity;
		std::string type;
		bool visible;
		int width;
		int x;
		int y;
	};
}

#endif


