#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <cstdint>

struct Vertex
{
	glm::vec3 Position;
	glm::vec4 Color = {1.0f, 1.0f, 1.0f, 1.0f};
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};