#include <iostream>
#include <SDL2/SDL_image.h>
#include "BackendSDL2.h"
#include "ImageSDL2.h"
#include "../../Shared.h"


namespace jam
{
	ImageSDL2::ImageSDL2()
	{
		this->texture = nullptr;
	}

	ImageSDL2::~ImageSDL2()
	{
		if (this->texture != nullptr)
		{
			SDL_DestroyTexture(this->texture);
			this->texture = nullptr;
		}
	}

	bool ImageSDL2::Construct(std::string fileName)
	{
		this->texture = nullptr;
		this->texture = this->LoadTexture(fileName);
		return this->texture != nullptr;
	}

	int ImageSDL2::GetWidth()
	{
		SDL_Rect r;
		if (this->texture != nullptr)
		{

			SDL_QueryTexture(this->texture, NULL, NULL, &r.w, &r.h);
			return r.w;
		}
		else
			return 0;
	}

	SDL_Texture* ImageSDL2::GetHandle()
	{
		return this->texture;
	}

	int ImageSDL2::GetHeight()
	{
		SDL_Rect r;
		if (this->texture != nullptr)
		{

			SDL_QueryTexture(this->texture, NULL, NULL, &r.w, &r.h);
			return r.h;
		}
		else
			return 0;
	}

	SDL_Texture* ImageSDL2::LoadTexture(std::string fileName)
	{
		SDL_Texture* texture = nullptr;

		// Load image at specified path
		texture = IMG_LoadTexture(((BackendSDL2*) jam::backEnd)->GetRenderer(), fileName.c_str());
		if (texture == nullptr)
		{
			std::cerr << "Unable to load image \"" << fileName << "\" SDL Error: " << SDL_GetError() << std::endl;
		}

		return texture;
	}

}
