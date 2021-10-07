#pragma once
#include "Mesh.h"

#include <string>
#include <cstdint>

class Shader
{
public:
	enum class Stage {
		Vert1 = 0, Vert2 = 1, Vert3 = 2, Pixel = 3
	};

	void SetStage(Stage s) { stage = s; }
	void SetInPSBuffer(const glm::vec3& val);

	virtual Vertex VertexShader(const Vertex& vert) = 0;
	virtual glm::vec4 PixelShader() = 0;

protected:
	struct LayoutElement
	{
		std::string name;
		size_t offset;
		enum class Type {
			Float, Int, Vec2, Vec3, Vec4
		} type;
	};

	template<typename T>
	T* GetVar(const std::string& name)
	{
		for (auto& e : vsOutLayout)
		{
			if (e.name == name)
			{
				if (stage != Stage::Pixel) {
					size_t offset = ((int)stage * outSize) + e.offset;
					return (T*)(outBuffer + offset);
				}
				else
					return (T*)(inBuffer+e.offset);
			}
		}
		return nullptr;
	}

	void SetLayout(const std::vector<LayoutElement>& layout);

private:
	std::vector<LayoutElement> vsOutLayout;
	Stage stage;
	size_t outSize;
	unsigned char* outBuffer = nullptr;
	unsigned char* inBuffer = nullptr;

};


class DefaultShader : public Shader
{
public:
	DefaultShader();

	virtual Vertex VertexShader(const Vertex& vert);
	virtual glm::vec4 PixelShader();
};