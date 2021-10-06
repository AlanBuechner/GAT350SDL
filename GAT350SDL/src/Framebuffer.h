#pragma once
#include "Renderer.h"
#include <vector>
#include <glm\glm.hpp>

class Mesh;

class Framebuffer
{
public:
	Framebuffer(Renderer* renderer, int width, int height);
	~Framebuffer();

	void Update();

	void Clear(const color_t& color);
	void DrawPoint(const glm::vec2& p, const color_t& color);
	void DrawRect(const glm::vec2& p, const glm::vec2& size, const color_t& color);
	void DrawLine(glm::vec2 p1, glm::vec2 p2, const color_t& color);
	void DrawCircle(const glm::vec2& p, int radius, const color_t& color);
	void DrawTriangle(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, const color_t& color, bool ndc = false);

	void DrawMesh(const Mesh& mesh, const color_t& color);

private:
	void DrawFlatTop(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const color_t& color);
	void DrawFlatBottom(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const color_t& color);

public:
	SDL_Texture* texture{ nullptr };
	uint8_t* buffer{ nullptr };

	int width = 0;
	int height = 0;
	int pitch = 0;
};
