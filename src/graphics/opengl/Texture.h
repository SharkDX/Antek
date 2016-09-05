#pragma once

#include <string>
#include <GL/glew.h>

namespace Antek
{
	namespace Graphics
	{
		namespace OpenGL
		{
			class Texture
			{
			public:
				Texture(const std::string& path);
				Texture(Texture& texture) = delete;
				Texture& operator=(const Texture& texture) = delete;
				Texture(Texture&& other);
				Texture& operator=(Texture&& other) = delete; // TODO ?

				void Bind();

				int GetWidth() const;
				int GetHeight() const;

				~Texture();

			private:
				GLuint _texture;
				int _width;
				int _height;

				void LoadTexture(const std::string& path);
			};
		}
	}
}