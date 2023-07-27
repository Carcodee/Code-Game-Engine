#pragma once
#include "GL/glew.h"

class VAO
{
public:
	VAO();
	void Bind();
	void UnBind();
	~VAO();
	
private:
	GLuint id=0;
};

