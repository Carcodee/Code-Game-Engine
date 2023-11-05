#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Framebuffer
{
	public:
		Framebuffer();
		void generateTexture();
		void Bind();
		void UnBind();
		void Resize(int width, int height);
		~Framebuffer();
		GLuint m_Texture=0; 
		void InitPickingFramebuffer(GLFWwindow* window);
		int m_Width = 1280;
		int m_Height = 720;
	private:
		GLuint m_Fbo = 0;
		GLuint m_Rbo = 0;

};

