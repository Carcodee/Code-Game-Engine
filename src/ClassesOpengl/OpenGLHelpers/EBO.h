#pragma once
#include "GL/glew.h"
class EBO{

public:
	EBO();
	void Bind();
	void BufferData(GLsizeiptr size, const void* data);
	void UnBind();
	~EBO();

private:
	GLuint id = 0;
};

