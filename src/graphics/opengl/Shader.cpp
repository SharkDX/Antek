#include "Shader.h"


Antek::Graphics::OpenGL::Shader::Shader()
{

}

Antek::Graphics::OpenGL::Shader::Shader(const char* pathVertex, const char* pathFragment)
{
	LoadFromFile(pathVertex, pathFragment);
}

bool Antek::Graphics::OpenGL::Shader::LoadFromFile(const char *pathVertex, const char *pathFragment)
{
	_vertex_shader_obj = glCreateShader(GL_VERTEX_SHADER);
	_fragment_shader_obj = glCreateShader(GL_FRAGMENT_SHADER);
	_program = glCreateProgram();
	std::ifstream fileVer;
	std::ifstream fileFrag;

	fileVer.open(pathVertex);

	if (fileVer.is_open())
	{
		std::string buffer;

		while (fileVer.good())
		{
			std::getline(fileVer, buffer);
			_vertex_source.append(buffer + "\n");

		}

		fileVer.close();
	}
	else
	{
		std::cout << "Cannot open shader file: " << pathVertex << std::endl;
		return false;
	}

	fileFrag.open(pathFragment);

	if (fileFrag.is_open())
	{
		std::string buffer;

		while (fileFrag.good())
		{
			getline(fileFrag, buffer);
			_fragment_source.append(buffer + "\n");
		}

		fileFrag.close();
	}
	else
	{
		std::cout << "Cannot open shader file: " << pathFragment << std::endl;
		return false;
	}

	const char *vP = _vertex_source.c_str();
	const char *vF = _fragment_source.c_str();

	glShaderSource(_vertex_shader_obj, 1, &vP, NULL);
	glShaderSource(_fragment_shader_obj, 1, &vF, NULL);
	glCompileShader(_vertex_shader_obj);
	GLint Result;
	GLint InfoLogLength;
	glGetShaderiv(_vertex_shader_obj, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		glGetShaderiv(_vertex_shader_obj, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(_vertex_shader_obj, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}

	glCompileShader(_fragment_shader_obj);
	glGetShaderiv(_fragment_shader_obj, GL_COMPILE_STATUS, &Result);

	if (Result == GL_FALSE)
	{
		glGetShaderiv(_fragment_shader_obj, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage = std::vector<char>(InfoLogLength);
		glGetShaderInfoLog(_fragment_shader_obj, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}
	
	glAttachShader(_program, _vertex_shader_obj);
	glAttachShader(_program, _fragment_shader_obj);
	glLinkProgram(_program);
	glDeleteShader(_vertex_shader_obj);
	glDeleteShader(_fragment_shader_obj);
	GLint numActiveAttribs = 0;
	GLint numActiveUniforms = 0;
	glGetProgramiv(_program, GL_ACTIVE_ATTRIBUTES, &numActiveAttribs);
	glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
	std::vector<GLchar> nameData(256);

	for (int unif = 0; unif < numActiveUniforms; ++unif)
	{
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		glGetActiveUniform(_program, unif, nameData.size(), &actualLength, &arraySize, &type, &nameData[0]);
		std::string name((char*) &nameData[0], actualLength);
		_uniforms.insert(std::pair<std::string, GLuint>(name, glGetUniformLocation(_program, name.c_str())));
	}

	return true;
}

void Antek::Graphics::OpenGL::Shader::Bind()
{
	glUseProgram(_program);
}

void Antek::Graphics::OpenGL::Shader::Release()
{
	glUseProgram(0);
}

GLuint Antek::Graphics::OpenGL::Shader::GetUniformLocation(const std::string &name)
{
	std::map<std::string, GLuint>::iterator it = _uniforms.find(name);
	return it == _uniforms.end() ? -1 : it->second;
}

void Antek::Graphics::OpenGL::Shader::SetUniform(GLuint loc, float value)
{
	glUniform1f(loc, value);
}

void Antek::Graphics::OpenGL::Shader::SetUniform(GLuint loc, float value1, float value2)
{
	glUniform2f(loc, value1, value2);
}

void Antek::Graphics::OpenGL::Shader::SetUniform(GLuint loc, float value1, float value2, float value3)
{
	glUniform3f(loc, value1, value2, value3);
}

void Antek::Graphics::OpenGL::Shader::SetUniform(GLuint loc, glm::vec2 value)
{
	glUniform2f(loc, value.x, value.y);
}

void Antek::Graphics::OpenGL::Shader::SetUniform(GLuint loc, glm::vec3 value)
{
	glUniform3f(loc, value.x, value.y, value.z);
}

void Antek::Graphics::OpenGL::Shader::SetUniform(GLuint loc, glm::vec4 value)
{
	glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Antek::Graphics::OpenGL::Shader::SetUniform(GLuint loc, glm::mat4& value)
{
	glUniformMatrix4fv(loc, 1, false, &value[0][0]);
}

GLuint Antek::Graphics::OpenGL::Shader::getAttributeLocation(const char *name)
{
	return glGetAttribLocation(_program, name);
}

void Antek::Graphics::OpenGL::Shader::BindAttributeLocation(const char *name, int loc)
{
	glBindAttribLocation(_program, loc, name);
}

