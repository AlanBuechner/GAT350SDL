#pragma once
#include <vector>
#include <glm\glm.hpp>
#include <cstdint>

struct Vertex
{
	glm::vec3 Position;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};