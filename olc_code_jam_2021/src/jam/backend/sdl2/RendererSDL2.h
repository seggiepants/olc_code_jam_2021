#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "../../IRenderer.h"

namespace jam
{
    class RendererSDL2 :
        public IRenderer
    {
    public:
        void Init(SDL_Window* window, SDL_Renderer* renderer);
        void Clear(rgb bg);
        void DrawImage(IImage* img, int x, int y);
        void DrawLine(int x1, int y1, int x2, int y2, rgb color);
        void DrawPoint(int x1, int y1, rgb color);
        void DrawPolygon(float x, float y, std::vector<jam::Point2Df>* points, jam::rgb color);
        void DrawSubImage(IImage* img, int screenX, int screenY, int x, int y, int w, int h);
        void FillRect(int x1, int y1, int x2, int y2, rgb color);
        void GetScreenSize(int* width, int* height);
    private:        
        SDL_Renderer* renderer;
        SDL_Window* window;
    };

}
