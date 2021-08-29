#pragma once
#ifndef __TILE_SET_H__
#define __TILE_SET_H__

#include <string>
#include "../3rdParty/xml/tinyxml2.h"

namespace jam
{
	class TileSet
	{
	public:
		TileSet();
		~TileSet();
		void Construct(std::string fileName);
		std::string ImageKey();
		int GetWidth() { return this->width; };
		int GetHeight() { return this->height; };
		int GetTileWidth() { return this->tileWidth; };
		int GetTileHeight() { return this->tileHeight; };

	protected:
		std::string name;
		std::string source;
		int width;
		int height;
		int tileWidth;
		int tileHeight;
		int tileCount;
		int columns;
	};
}

#endif