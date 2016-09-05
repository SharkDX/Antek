#pragma once
#include <GL/glew.h>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <assert.h>
#include "glm/glm.hpp"

namespace Antek
{
	namespace Graphics
	{
		namespace OpenGL
		{
			class Shader
			{
			public:
				Shader();
				Shader(const char* pathVertex, const char* pathFragment);
				bool LoadFromFile(const char *pathVertex, const char *pathFragment);
				void Bind();
				void Release();
				GLuint GetUniformLocation(const std::string& name);
				GLuint GetProgram() { return _program; };

				void SetUniform(GLuint loc, float value);
				void SetUniform(GLuint loc, float value1, float value2);
				void SetUniform(GLuint loc, float value1, float value2, float value3);
				void SetUniform(GLuint loc, glm::vec2 value);
				void SetUniform(GLuint loc, glm::vec3 value);
				void SetUniform(GLuint loc, glm::vec4 value);
				void SetUniform(GLuint loc, glm::mat4& value);

				GLuint getAttributeLocation(const char *name);
				void BindAttributeLocation(const char *name, int loc);
			private:
				GLuint _program;
				GLuint _vertex_shader_obj;
				GLuint _fragment_shader_obj;

				std::string _vertex_source;
				std::string _fragment_source;

				std::map<std::string, GLuint> _uniforms;
			};
		}
	}
}