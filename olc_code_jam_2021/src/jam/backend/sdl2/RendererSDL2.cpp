#include <iostream>
#include <SDL2/SDL_image.h>
#include "RendererSDL2.h"
#include "ImageSDL2.h"

namespace jam
{
    void RendererSDL2::Init(SDL_Window* window, SDL_Renderer* renderer)
    {
        this->window = window;
        this->renderer = renderer;
    }
    void RendererSDL2::Clear(rgb bg)
    {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(this->renderer, bg.r, bg.g, bg.b, bg.a);
        SDL_RenderClear(this->renderer);
        SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    }

    void RendererSDL2::DrawImage(IImage* img, int x, int y)
    {
        SDL_Rect dest;
        SDL_Texture* texture = ((ImageSDL2*)img)->GetHandle();
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
        dest.x = x;
        dest.y = y;
        SDL_RenderCopy(this->renderer, texture, NULL, &dest);
    }

    void RendererSDL2::DrawLine(int x1, int y1, int x2, int y2, rgb color)
    {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawLine(this->renderer, x1, y1, x2, y2);
        SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    }

    void RendererSDL2::DrawPoint(int x, int y, rgb color)
    {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
        SDL_RenderDrawPoint(this->renderer, x, y);
        SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    }

    void RendererSDL2::DrawPolygon(float x, float y, std::vector<jam::Point2Df>* points, jam::rgb color)
    {
        SDL_Point* polygonPoints = new SDL_Point[points->size() + 1];
        for (int i = 0; i < points->size(); i++)
        {
            polygonPoints[i].x = points->at(i).x + x;
            polygonPoints[i].y = points->at(i).y + y;
        }
        // Connect last point to first.
        polygonPoints[points->size()].x = polygonPoints[0].x;
        polygonPoints[points->size()].y = polygonPoints[0].y;
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);

        SDL_RenderDrawLines(this->renderer, polygonPoints, points->size() + 1);

        SDL_SetRenderDrawColor(this->renderer, r, g, b, a);

        delete[] polygonPoints;
    }

    void RendererSDL2::DrawSubImage(IImage* img, int screenX, int screenY, int x, int y, int w, int h)
    {
        SDL_Rect src, dest;
        SDL_Texture* texture = ((ImageSDL2*)img)->GetHandle();
        dest.x = screenX;
        dest.y = screenY;
        dest.w = w;
        dest.h = h;
        src.x = x;
        src.y = y;
        src.w = w;
        src.h = h;
        SDL_RenderCopy(this->renderer, texture, &src, &dest);
    }

    void RendererSDL2::FillRect(int x1, int y1, int x2, int y2, rgb color)
    {
        Uint8 r, g, b, a;
        SDL_GetRenderDrawColor(this->renderer, &r, &g, &b, &a);
        SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
        SDL_Rect rect;
        rect.x = x1 > x2 ? x2 : x1;
        rect.y = y1 > y2 ? y2 : y1;
        rect.w = abs(x2 - x1);
        rect.h = abs(y2 - y1);
        SDL_RenderFillRect(this->renderer, &rect);
        SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    }

    void RendererSDL2::GetScreenSize(int* width, int* height)
    {
        SDL_GetWindowSize(this->window, width, height);
    }
}