#include "Framebuffer.h"
#include "glm\glm.hpp"

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

void Framebuffer::DrawPoint(glm::vec2 p, const color_t& color)
{
	if (p.x < 0 || p.x > width || p.y < 0 || p.y > height)
		return;

	((color_t*)buffer)[(int)p.y * width + (int)p.x] = color;
}

void Framebuffer::DrawRect(glm::vec2 p, glm::vec2 size, const color_t& color)
{
	for (float sx = p.x; sx < size.x + p.x; sx++)
		for (float sy = p.y; sy < size.y + p.y; sy++)
			DrawPoint({ sx, sy }, { 255,255,255,255 });
}

void Framebuffer::DrawLine(glm::vec2 p1, glm::vec2 p2, const color_t& color)
{
	float m = 0.0f;
	if (p1.x != p2.x)
		m = ((float)p2.y - (float)p1.y) / ((float)p2.x - (float)p1.x);


	if (p1.x != p2.x && std::abs(m) <= 1.0f)
	{
		if (p1.x > p2.x)
		{
			glm::vec2 temp = p1;
			p1 = p2; p2 = temp;
		}

		const float b = p1.y - m * p1.x;

		for (float sx = p1.x; sx < p1.x; sx++)
		{
			const float sy = m * sx + b;
			DrawPoint({ sx, sy }, color);
		}
	}
	else
	{
		if (p1.y > p2.y)
		{
			glm::vec2 temp = p1;
			p1 = p2; p2 = temp;
		}

		const float w = (p2.x - p1.x) / (p2.y - p1.y);
		const float b = p1.x - w * p1.y;

		for (float sy = p1.y; sy < p2.y; sy++)
		{
			const float sx = w * sy + b;
			DrawPoint({ sx, sy }, color);
		}
	}
}

void Framebuffer::DrawCircle(glm::vec2 p, int radius, const color_t& color)
{

	float x1 = p.x - radius, x2 = p.x + radius;
	float y1 = p.y - radius, y2 = p.y + radius;

	for (float sx = x1; sx < x2; sx++)
	{
		for (float sy = y1; sy < y2; sy++)
		{
			float distsqar = (sx - p.x) * (sx - p.x) + (sy - p.y) * (sy - p.y);
			if (distsqar <= radius * radius)
				DrawPoint({ sx, sy }, color);
		}
	}

}

void Framebuffer::DrawTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, const color_t& color, bool ndc)
{
	if (ndc)
	{
		// convert from ndc space to screen space
		p1.x = (p1.x + 1) * width / 2;
		p1.y = (p1.y + 1) * height / 2;
		p2.x = (p2.x + 1) * width / 2;
		p2.y = (p2.y + 1) * height / 2;
		p3.x = (p3.x + 1) * width / 2;
		p3.y = (p3.y + 1) * height / 2;
	}

	glm::vec2* v1 = &p1;
	glm::vec2* v2 = &p2;
	glm::vec2* v3 = &p3;

	if (v2->y < v1->y) std::swap(v1,v2);
	if (v3->y < v2->y) std::swap(v2,v3);
	if (v2->y < v1->y) std::swap(v1,v2);

	if (v1->y == v2->y) // flat top
	{
		if (v2->x < v1->x) std::swap(v1, v2);
		DrawFlatTop(*v1, *v2, *v3, color);
	}
	else if (v2->y == v3->y) // flat bottom
	{
		if (v3->x < v2->x) std::swap(v2, v3);
		DrawFlatBottom(*v1, *v2, *v3, color);
	}
	else
	{
		const float alphaSplit = (v2->y - v1->y) / (v3->y - v1->y);
		const glm::vec2 vi = *v1 + (*v3-*v1) * alphaSplit;

		if (v2->x < vi.x)
		{
			DrawFlatBottom(*v1, *v2, vi, color);
			DrawFlatTop(*v2, vi, *v3, color);
		}
		else
		{
			DrawFlatBottom(*v1, vi, *v2, color);
			DrawFlatTop(vi, *v2, *v3, color);
		}
	}

}

void Framebuffer::DrawFlatTop(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, const color_t& color)
{
	float m1 = (p3.x - p1.x) / (p3.y - p1.y);
	float m2 = (p3.x - p2.x) / (p3.y - p2.y);

	const int yStart = (int)ceil(p1.y-0.5f);
	const int yEnd = (int)ceil(p3.y-0.5f);

	for (int y = yStart; y < yEnd; y++)
	{
		const float px1 = m1 * (float(y) + 0.5f - p1.y) + p1.x;
		const float px2 = m2 * (float(y) + 0.5f - p2.y) + p2.x;

		const int xStart = (int)ceil(px1 - 0.5f);
		const int xEnd = (int)ceil(px2 - 0.5f);

		for (int x = xStart; x < xEnd; x++)
			DrawPoint({ x,y }, color);
	}

}

void Framebuffer::DrawFlatBottom(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, const color_t& color)
{
	float m1 = (p2.x - p1.x) / (p2.y - p1.y);
	float m2 = (p3.x - p1.x) / (p3.y - p1.y);

	const int yStart = (int)ceil(p1.y - 0.5f);
	const int yEnd = (int)ceil(p3.y - 0.5f);

	for (int y = yStart; y < yEnd; y++)
	{
		const float px1 = m1 * (float(y) + 0.5f - p1.y) + p1.x;
		const float px2 = m2 * (float(y) + 0.5f - p1.y) + p1.x;

		const int xStart = (int)ceil(px1 - 0.5f);
		const int xEnd = (int)ceil(px2 - 0.5f);

		for (int x = xStart; x < xEnd; x++)
			DrawPoint({ x,y }, color);
	}
}
