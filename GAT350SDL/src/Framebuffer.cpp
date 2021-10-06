#include "Framebuffer.h"

Framebuffer::Framebuffer(Renderer* renderer, int width, int height)
{
    this->width = width;
    this->height = height;

    texture = SDL_CreateTexture(renderer->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    pitch = width * sizeof(color_t);

    buffer = new uint8_t[(uint64_t)pitch * (uint64_t)height];
}

Framebuffer::~Framebuffer()
{
    SDL_DestroyTexture(texture);
    delete[] buffer;
}

void Framebuffer::Update()
{
    SDL_UpdateTexture(texture, nullptr, (const void*)buffer, pitch);
}

void Framebuffer::Clear(const color_t& color)
{
    for (int i = 0; i < width * height; i++)
        ((color_t*)buffer)[i] = color;
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
    if (x < 0 || x > width || y < 0 || y > height)
        return;

    ((color_t*)buffer)[y*width+x] = color;
}

void Framebuffer::DrawRect(int x, int y, int width, int height, const color_t& color)
{
    for (int sx = x; sx < width+x; sx++)
        for (int sy = y; sy < height+y; sy++)
            DrawPoint(sx, sy, { 255,255,255,255 });
}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
    float m = 0.0f;
    if (x1 != x2)
        m = ((float)y2 - (float)y1) / ((float)x2 - (float)x1);
    

    if (x1 != x2 && std::abs(m) <= 1.0f)
    {
        if (x1 > x2)
        {
            float temp = x1;
            x1 = x2; x2 = temp;
            temp = y1;
            y1 = y2; y2 = temp;
        }

        const float b = y1 - m * x1;

        for (int sx = x1; sx < x2; sx++)
        {
            const float sy = m * sx + b;
            DrawPoint(sx, sy, color);
        }
    }
    else
    {
        if (y1 > y2)
        {
            float temp = y1;
            y1 = y2; y2 = temp;
            temp = x1;
            x1 = x2; x2 = temp;
        }

        const float w = ((float)x2 - (float)x1) / ((float)y2 - (float)y1);
        const float b = x1 - w * y1;

        for (int sy = y1; sy < y2; sy++)
        {
            const float sx = w * sy + b;
            DrawPoint(sx, sy, color);
        }
    }


}

void Framebuffer::DrawCircle(int x, int y, int radius, const color_t& color)
{
}
