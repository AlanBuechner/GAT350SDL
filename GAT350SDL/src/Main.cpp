#include "Renderer.h"
#include "Framebuffer.h"
#include "Mesh.h"
#include "Shader.h"

#include <iostream>
#include <SDL.h>

int main(int, char**)
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
	renderer->Initialize(WIDTH, HEIGHT);

	std::unique_ptr<Framebuffer> frameBuffer = std::make_unique<Framebuffer>(renderer.get(), WIDTH, HEIGHT);
	Mesh mesh;
	mesh.vertices = std::vector<Vertex>{	{ { 0.5f,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} }, 
											{ {-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
											{ { 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
											{ {-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
	};
	mesh.indices = {0,1,2, 1,0,3};

	DefaultShader shader;
	
	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		frameBuffer->Clear({ 0,0,0,0 });
		frameBuffer->DrawRect({ 10, 10 }, { 100, 100 }, { 255,0,0,255 });
		frameBuffer->DrawLine({ 200, 200 }, { 300, 400 }, { 0,255,0,255 });
		frameBuffer->DrawCircle({ 600, 200 }, 100, { 0,0,255,255 });
		frameBuffer->DrawMesh(mesh, { 1.0f,1.0f,1.0f,1.0f }, shader);
		frameBuffer->Update();

		renderer->CopyBuffer(frameBuffer.get());
		renderer->Present();
	}

	SDL_Quit();

	return 0;
}
