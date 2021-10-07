#include "Shader.h"
#include <glm\glm.hpp>

void Shader::SetInPSBuffer(const glm::vec3& val)
{
	for (auto e : vsOutLayout)
	{
		switch (e.type)
		{
		case LayoutElement::Type::Int:
		{
			*(int*)(inBuffer + e.offset) = *(int*)(outBuffer + e.offset);
			break;
		}
		case LayoutElement::Type::Float:
		{
			float& v1 = *(float*)(outBuffer + e.offset + (0*outSize));
			float& v2 = *(float*)(outBuffer + e.offset + (1*outSize));
			float& v3 = *(float*)(outBuffer + e.offset + (2*outSize));
			float& ov = *(float*)(inBuffer + e.offset);
			ov = v1 * val.x + v2 * val.y + v3 * val.z;
		}
		case LayoutElement::Type::Vec2:
		{
			glm::vec2& v1 = *(glm::vec2*)(outBuffer + e.offset + (0 * outSize));
			glm::vec2& v2 = *(glm::vec2*)(outBuffer + e.offset + (1 * outSize));
			glm::vec2& v3 = *(glm::vec2*)(outBuffer + e.offset + (2 * outSize));
			glm::vec2& ov = *(glm::vec2*)(inBuffer + e.offset);
			ov = v1 * val.x + v2 * val.y + v3 * val.z;
		}
		case LayoutElement::Type::Vec3:
		{
			glm::vec3& v1 = *(glm::vec3*)(outBuffer + e.offset + (0 * outSize));
			glm::vec3& v2 = *(glm::vec3*)(outBuffer + e.offset + (1 * outSize));
			glm::vec3& v3 = *(glm::vec3*)(outBuffer + e.offset + (2 * outSize));
			glm::vec3& ov = *(glm::vec3*)(inBuffer + e.offset);
			ov = v1 * val.x + v2 * val.y + v3 * val.z;
		}
		case LayoutElement::Type::Vec4:
		{
			glm::vec4& v1 = *(glm::vec4*)(outBuffer + e.offset + (0 * outSize));
			glm::vec4& v2 = *(glm::vec4*)(outBuffer + e.offset + (1 * outSize));
			glm::vec4& v3 = *(glm::vec4*)(outBuffer + e.offset + (2 * outSize));
			glm::vec4& ov = *(glm::vec4*)(inBuffer + e.offset);
			ov = v1 * val.x + v2 * val.y + v3 * val.z;
		}
		default:
			break;
		}
	}
}

void Shader::SetLayout(const std::vector<LayoutElement>& layout)
{
	vsOutLayout = layout;
	size_t size = 0;
	for (auto &e : vsOutLayout)
	{
		e.offset = size;
		switch (e.type)
		{
		case LayoutElement::Type::Int: size += sizeof(int); break;
		case LayoutElement::Type::Float: size += sizeof(float); break;
		case LayoutElement::Type::Vec2: size += sizeof(glm::vec2); break;
		case LayoutElement::Type::Vec3: size += sizeof(glm::vec3); break;
		case LayoutElement::Type::Vec4: size += sizeof(glm::vec4); break;
		default:
			break;
		}
	}

	outSize = size;
	outBuffer = new unsigned char[size*3];
	inBuffer = new unsigned char[size];
	stage = Stage::Vert1;
}


// ------- Default Shader ------- //
DefaultShader::DefaultShader()
{
	SetLayout({
		{"Color", 0, LayoutElement::Type::Vec4}
	});
}

Vertex DefaultShader::VertexShader(const Vertex& vert)
{
	glm::vec4& color = GetVar<glm::vec4>("Color");
	color = vert.Color;
	return vert;
}

glm::vec4 DefaultShader::PixelShader()
{
	return GetVar<glm::vec4>("Color");
}
