#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
}


void Framebuffer::generateTexture()
{
	if (m_Fbo)
	{
		glDeleteFramebuffers(1, &m_Fbo);
		glDeleteTextures(1, &m_Texture);
	}

	//int width, height;
	//glfwGetWindowSize(window, &m_Width, &m_Height);
	glCreateFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

	glCreateTextures(GL_TEXTURE_2D,1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	//TODO Enum to handle all the different types of resolutions
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);

	//render buffer obj
	glGenRenderbuffers(1, &m_Rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_Rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1280, 720); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
	glViewport(0,0, m_Width, m_Height);
}

void Framebuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(int width, int height)
{
	this->m_Width = width;
	this->m_Height = height;
	generateTexture();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_Fbo);
	glDeleteTextures(1, &m_Texture);

}

void Framebuffer::InitPickingFramebuffer(GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glGenFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,m_Fbo);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
