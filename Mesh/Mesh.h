#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Shaders/ShaderClass.h"
#include "../ClassesOpengl/VAO.h"
#include "../ClassesOpengl/VBO.h"
#include "stb-master/stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh
{
public:
	 VAO myVAO;
	 VBO myVBO;
	 Mesh(std::vector <GLfloat> &vertices, float screenW, float screenH);
	 void SetTexture(GLuint& textures, const char* filepath, bool isRGBA);
	 void Render(ShaderClass& shader, GLenum mode, GLint first, GLsizei count);
	 void Render(ShaderClass& shader, GLenum mode, GLint first, GLsizei count, GLsizei instancecount);

	 void SetAtrib(GLuint index,GLint size,GLenum type,GLboolean normalized,GLsizei stride,const void* pointer);

private:
	float screenH;
	float screenW;
};

