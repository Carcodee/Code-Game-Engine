#include "EBO.h"

EBO::EBO()
{
	glGenBuffers(1, &id);
}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::BufferData(GLsizeiptr size, const void* data)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

void EBO::UnBind()
{
	glBindVertexArray(0);
}

EBO::~EBO()
{
	glDeleteVertexArrays(1, &id);
}
