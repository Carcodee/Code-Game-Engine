#include "Mesh.h"

Mesh::Mesh(std::vector <GLfloat>& vertices, float screenW, float screenH)
{
	this->screenW = screenW;
	this->screenH = screenH;

	myVAO.Bind();
	//vbo settings
	myVBO.Bind();
	myVBO.BufferData(vertices.size(), vertices.data());


}

void Mesh::SetTexture(GLuint& textures,const char* filepath, bool isRGBA)
{
	//textures
	glGenTextures(1, &textures);
	glBindTexture(GL_TEXTURE_2D, textures);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

	if (!isRGBA)
	{
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

	}
	else
	{
		if (data)
		{
			// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

}

//Single calls
void Mesh::Render(ShaderClass& shader, GLenum mode,GLint first, GLsizei count)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0, 0, 3));
	shader.setMat4("model", model);
	myVAO.Bind();
	glDrawArrays(mode, first, count);

}



void Mesh::SetAtrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
	myVAO.Bind();
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(0);

}
