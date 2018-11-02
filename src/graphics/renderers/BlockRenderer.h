#pragma once
#define BUFFER_OFFSET(i) ((void*)(i))
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include "..\..\Block.h"
#include "..\..\World.h"
#include "..\opengl\Shader.h"
#include "..\opengl\Vertex.h"
#include "..\..\BlockFactory.h"

using namespace std;
using namespace Antek::Graphics;

namespace Antek
{
	namespace Graphics
	{
		namespace Renderers
		{
			class BlockRenderer
			{
			public:				
				void Init(Block**** blocks, const uint32_t x_size, const uint32_t y_size, const uint32_t z_size);
				void AddVertices(const int x, const int y, const int z, std::vector<OpenGL::Vertex>& v, const uint8_t solid_state, const glm::vec3 color, float light[6]);
				void Render(glm::mat4 &mvpMatrix);
			private:
				OpenGL::Shader* _shader;
				GLuint _vbo;
				GLuint _vbo_length;
			};
		}
	}
}

