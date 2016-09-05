#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

namespace Antek
{
	namespace Graphics
	{
		namespace OpenGL
		{
			struct Vertex
			{
			public:
				glm::vec3 Position, Normal, Color;

				Vertex(glm::vec3 pos, glm::vec3 normal) 
					: Position(pos), Normal(normal)
				{
				}

				Vertex(const float &x, const float &y, const float &z, const float &nX, const float &nY, const float &nZ, const glm::vec3 color) 
					: Position(glm::vec3(x, y, z)), Normal(glm::vec3(nX, nY, nZ)),  Color(color)
				{
				}

				Vertex(const float &x, const float &y, const float &z, const float &nX, const float &nY, const float &nZ)
					: Position(glm::vec3(x, y, z)), Normal(glm::vec3(nX, nY, nZ))
				{
				}

				Vertex(const glm::vec3 &pos, const float &nX, const float &nY, const float &nZ)
					: Position(pos), Normal(glm::vec3(nX, nY, nZ))
				{
				}

				static int GetSizeInByte()
				{
					return 36;
				}
			};
		}
	}
}