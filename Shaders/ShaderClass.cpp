#include "ShaderClass.h"

ShaderClass::ShaderClass(const char* vertexPath, const char* fragmentPath)
{

	//Loading shaders from file

	//Vertex Shaders
	std::string vertexShaderSource;

	//Fragment Shaders
	std::string fragmentShaderSource;


	//file streams
	std::ifstream vertexShaderStream;
	std::ifstream fragmentShaderStream;

	try
	{
		vertexShaderStream.open(vertexPath);
		fragmentShaderStream.open(fragmentPath);

		std::stringstream vertexShaderStrStream;
		std::stringstream fragmentShaderStrStream;

		vertexShaderStrStream << vertexShaderStream.rdbuf();
		fragmentShaderStrStream << fragmentShaderStream.rdbuf();

		vertexShaderStream.close();
		fragmentShaderStream.close();


		vertexShaderSource = vertexShaderStrStream.str();
		fragmentShaderSource = fragmentShaderStrStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vertexShaderCode = vertexShaderSource.c_str();
	const char* fragmentShaderCode = fragmentShaderSource.c_str();




	//Shader Config, vertex shader


	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	//Shader Config, Fragment shader

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::FRAGMENT::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//ShaderProgram

	ID= glCreateProgram();

	//This attach the vShader and the FShader and then is linked with the glLinked. If something is not right linked will return an error
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//Error checker
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR IN THE LINKING OF THE SHADER PROGRAM\n" << infoLog << std::endl;
	}

	//
}

void ShaderClass::use()
{
	glUseProgram(ID);
}

void ShaderClass::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	
}


void ShaderClass::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderClass::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void ShaderClass::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
