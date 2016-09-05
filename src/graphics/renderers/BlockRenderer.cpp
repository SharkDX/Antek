#include "BlockRenderer.h"

void Antek::Graphics::Renderers::BlockRenderer::Init(Block**** blocks, const uint32_t x_size, const uint32_t y_size, const uint32_t z_size)
{
	_shader = new OpenGL::Shader("Shaders/blocks.vert", "Shaders/blocks.frag");

	std::vector<OpenGL::Vertex> vertices;
	for (int i = 0; i < x_size; i++)
	{
		for (int k = 0; k < z_size; k++)
		{
			for (int j = 0; j < y_size; j++)
			{
				if (blocks[i][k][j] != nullptr)
				{
					uint8_t solid_state = 
						  (k < z_size - 1	&& !blocks[i][k + 1][j]) << 0
						| (k > 0			&& !blocks[i][k - 1][j]) << 1
						| (j < y_size - 1	&& !blocks[i][k][j + 1]) << 2
						| (j > 0			&& !blocks[i][k][j - 1]) << 3
						| (i < x_size - 1	&& !blocks[i + 1][k][j]) << 4
						| (i > 0			&& !blocks[i - 1][k][j]) << 5;

					if (solid_state)
					{
						glm::vec3 color = BlockFactory::GetBlockColor(blocks[i][k][j]->id);
						this->AddVertices(i, j, k, vertices, solid_state, color);
					}
				}
			}
		}
	}
	_vbo_length = vertices.size() * OpenGL::Vertex::GetSizeInByte();

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	if (_vbo_length > 0)
		glBufferData(GL_ARRAY_BUFFER, _vbo_length, &vertices[0], GL_STATIC_DRAW);

	printf("Vertices: %g\n", (float)_vbo_length / OpenGL::Vertex::GetSizeInByte());
}

void Antek::Graphics::Renderers::BlockRenderer::AddVertices(const int x, const int y, const int z, std::vector<OpenGL::Vertex>& v, const uint8_t solid_state, const glm::vec3 color)
{
	//Front
	if (solid_state & 1)
	{
		v.push_back(OpenGL::Vertex(x, y, z + 1, 0, 0, 1, color));
		v.push_back(OpenGL::Vertex(x + 1, y + 1, z + 1, 0, 0, 1, color));
		v.push_back(OpenGL::Vertex(x, y + 1, z + 1, 0, 0, 1, color));

		v.push_back(OpenGL::Vertex(x, y, z + 1, 0, 0, 1, color));
		v.push_back(OpenGL::Vertex(x + 1, y, z + 1, 0, 0, 1, color));
		v.push_back(OpenGL::Vertex(x + 1, y + 1, z + 1, 0, 0, 1, color));
	}

	//Back
	if (solid_state & 2)
	{
		v.push_back(OpenGL::Vertex(x, y, z, 0, 0, -1, color));
		v.push_back(OpenGL::Vertex(x, y + 1, z, 0, 0, -1, color));
		v.push_back(OpenGL::Vertex(x + 1, y + 1, z, 0, 0, -1, color));

		v.push_back(OpenGL::Vertex(x, y, z, 0, 0, -1, color));
		v.push_back(OpenGL::Vertex(x + 1, y + 1, z, 0, 0, -1, color));
		v.push_back(OpenGL::Vertex(x + 1, y, z, 0, 0, -1, color));
	}

	//Top
	if (solid_state & 4)
	{
		v.push_back(OpenGL::Vertex(x, y + 1, z, 0, 1, 0, color));
		v.push_back(OpenGL::Vertex(x, y + 1, z + 1, 0, 1, 0, color));
		v.push_back(OpenGL::Vertex(x + 1, y + 1, z + 1, 0, 1, 0, color));

		v.push_back(OpenGL::Vertex(x, y + 1, z, 0, 1, 0, color));
		v.push_back(OpenGL::Vertex(x + 1, y + 1, z + 1, 0, 1, 0, color));
		v.push_back(OpenGL::Vertex(x + 1, y + 1, z, 0, 1, 0, color));
	}

	//Buttom
	if (solid_state & 8)
	{
		v.push_back(OpenGL::Vertex(x, y, z, 0, -1, 0, color));
		v.push_back(OpenGL::Vertex(x + 1, y, z, 0, -1, 0, color));
		v.push_back(OpenGL::Vertex(x + 1, y, z + 1, 0, -1, 0, color));

		v.push_back(OpenGL::Vertex(x, y, z, 0, -1, 0, color));
		v.push_back(OpenGL::Vertex(x + 1, y, z + 1, 0, -1, 0, color));
		v.push_back(OpenGL::Vertex(x, y, z + 1, 0, -1, 0, color));
		
	}

	//Right
	if (solid_state & 16)
	{
		v.push_back(OpenGL::Vertex(x + 1, y, z, 1, 0, 0, color));
		v.push_back(OpenGL::Vertex(x + 1, y + 1, z, 1, 0, 0, color));
		v.push_back(OpenGL::Vertex(x + 1, y + 1, z + 1, 1, 0, 0, color));

		v.push_back(OpenGL::Vertex(x + 1, y, z, 1, 0, 0, color));
		v.push_back(OpenGL::Vertex(x + 1, y + 1, z + 1, 1, 0, 0, color));
		v.push_back(OpenGL::Vertex(x + 1, y, z + 1, 1, 0, 0, color));
	}

	//Left
	if (solid_state & 32)
	{
		v.push_back(OpenGL::Vertex(x, y, z, -1, 0, 0, color));
		v.push_back(OpenGL::Vertex(x, y, z + 1, -1, 0, 0, color));
		v.push_back(OpenGL::Vertex(x, y + 1, z + 1, -1, 0, 0, color));

		v.push_back(OpenGL::Vertex(x, y, z, -1, 0, 0, color));
		v.push_back(OpenGL::Vertex(x, y + 1, z + 1, -1, 0, 0, color));
		v.push_back(OpenGL::Vertex(x, y + 1, z, -1, 0, 0, color));
	}
}


void Antek::Graphics::Renderers::BlockRenderer::Render(glm::mat4 &mvpMatrix)
{
	_shader->Bind();
    
	_shader->SetUniform(_shader->GetUniformLocation("MVP"), mvpMatrix);

	_shader->BindAttributeLocation("pos", 0);
	_shader->BindAttributeLocation("normal", 1);
	_shader->BindAttributeLocation("color", 2);
	glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, OpenGL::Vertex::GetSizeInByte(), BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, OpenGL::Vertex::GetSizeInByte(), BUFFER_OFFSET(12));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, OpenGL::Vertex::GetSizeInByte(), BUFFER_OFFSET(24));

	glDrawArrays(GL_TRIANGLES, 0, _vbo_length / (OpenGL::Vertex::GetSizeInByte()));

	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	_shader->Release();
}
