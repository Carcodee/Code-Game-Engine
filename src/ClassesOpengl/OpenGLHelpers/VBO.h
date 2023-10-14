#pragma once
#include "GL/glew.h"

class VBO
{
public:
	VBO();
	void Bind();
	void BufferData(GLsizeiptr size, const void* data);
	void UnBind();
	~VBO();

private:
	GLuint id = 0;
};

