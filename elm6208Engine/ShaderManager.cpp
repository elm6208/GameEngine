#include "ShaderManager.h"



ShaderManager::ShaderManager()
{
	program = 0;
}


ShaderManager::~ShaderManager()
{

}

GLuint ShaderManager::getProgram() const
{
	return program;
}

bool ShaderManager::loadShaders(const char * vertexFile, const char* fragmentFile)
{
	GLuint result;
	result = loadShader(vertexFile, GL_VERTEX_SHADER);
	if (result == 0)
	{
		std::cout << "Error" << std::endl;
		return 0;
	}
	GLuint result2;
	result2 = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (result2 == 0)
	{
		std::cout << "Error" << std::endl;
		return 0;
	}
	program = glCreateProgram();
	glAttachShader(program, result);
	glAttachShader(program, result2);
	glLinkProgram(program);
	GLint linkRes;
	glGetProgramiv(program, GL_LINK_STATUS, &linkRes);
	if (linkRes != 0)
	{
		return true;
	}
	if (linkRes == 0)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(program, logLength, 0, infoLog);
		std::cout << infoLog << std::endl;
		glDeleteProgram(program);
		delete[] infoLog;
		return false;

	}
	return false;
}

GLuint ShaderManager::loadShader(const char * file, GLenum shaderType)
{
	std::ifstream inFileBinary(file, std::ios::binary);
	if (inFileBinary.is_open())
	{
		inFileBinary.seekg(0, std::ios::end);
		unsigned int length = (unsigned int)inFileBinary.tellg();
		inFileBinary.seekg(0, std::ios::beg);
		char* fileContents = new char[length + 1];
		inFileBinary.read(fileContents, length);
		fileContents[length] = '\0';

		GLuint index = glCreateShader(shaderType);
		glShaderSource(index, 1, &fileContents, 0);
		glCompileShader(index);
		delete[] fileContents;
		GLint result;
		glGetShaderiv(index, GL_COMPILE_STATUS, &result);
		if (result == 0)
		{
			GLint logLength;
			glGetShaderiv(index, GL_INFO_LOG_LENGTH, &logLength);
			GLchar* infoLog = new GLchar[logLength];
			glGetShaderInfoLog(index, logLength, 0, infoLog);
			std::cout << infoLog << std::endl;
			glDeleteShader(index);
			delete[] infoLog;
			return 0;
		}
		else {
			return index;
		}
	}
	
	return false;
}
