#include "VAO.h"
VAO::VAO()
{

	glGenVertexArrays(1, &id);
}

void VAO::Bind()
{
	glBindVertexArray(id);

}

void VAO::UnBind()
{
	glBindVertexArray(0);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &id);
}
