#include "VBO.h"

VBO::VBO()
{
	glGenBuffers(1, &id);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::BufferData(GLsizeiptr size, const void* data)
{
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

void VBO::UnBind()
{
	glBindVertexArray(0);
}

VBO::~VBO()
{

	glDeleteVertexArrays(1, &id);
}
