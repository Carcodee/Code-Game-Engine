#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Framebuffer
{
	public:
		Framebuffer();
		void generateTexture(GLFWwindow* window);
		void Bind();
		void UnBind();
		~Framebuffer();
		GLuint m_Texture;
		void InitPickingFramebuffer(GLFWwindow* window);
	private:
		GLuint m_Fbo;
		GLuint m_Rbo;
};

