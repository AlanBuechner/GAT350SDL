#pragma once
#include "Mesh.h"

#include <string>
#include <map>
#include <cstdint>

class Shader
{
public:
	enum class Stage {
		Vert1 = 0, Vert2 = 1, Vert3 = 2, Pixel = 3
	};

	~Shader();

	void SetStage(Stage s) { stage = s; }
	void SetInPSBuffer(const glm::vec3& val);

	virtual glm::vec4 VertexShader(const Vertex& vert) = 0;
	virtual glm::vec4 PixelShader() = 0;

	template<typename T>
	void SetUniformBuffer(int buffer, T& data)
	{
		if (uniformBuffer.find(buffer) != uniformBuffer.end())
			delete uniformBuffer.at(buffer);

		uniformBuffer[buffer] = new T(data);
	}

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
					return (T*)(vsOutBuffer + offset);
				}
				else
					return (T*)(psInBuffer +e.offset);
			}
		}
		return nullptr;
	}

	template<typename T>
	T* GetUniformBuffer(int buffer)
	{
		return (T*)uniformBuffer.at(buffer);
	}

	void SetLayout(const std::vector<LayoutElement>& layout);

private:
	std::vector<LayoutElement> vsOutLayout;
	Stage stage = Stage::Vert1;
	std::map<int, void*> uniformBuffer;

	size_t outSize = 0;
	unsigned char* vsOutBuffer = nullptr;
	unsigned char* psInBuffer = nullptr;

};


class DefaultShader : public Shader
{
public:
	DefaultShader();

	virtual glm::vec4 VertexShader(const Vertex& vert);
	virtual glm::vec4 PixelShader();
};