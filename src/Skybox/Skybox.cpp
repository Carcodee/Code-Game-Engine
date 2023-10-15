#include "Skybox.h"

void renderCube();
Skybox::Skybox(const char* filename)
{
	this->captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	this->captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	this->captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(-1.0f, 0.0f, 0.0f),glm::vec3(0.0f, -1.0f, 0.0f)));
	this->captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f)));
	this->captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, -1.0f, 0.0f),glm::vec3(0.0f, 0.0f, -1.0f)));
	this->captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f),glm::vec3(0.0f, -1.0f, 0.0f)));
	this->captureViews.push_back(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 0.0f, -1.0f),glm::vec3(0.0f, -1.0f, 0.0f)));


	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	float *data= stbi_loadf(filename, &width, &height, &nrChannels, 0);
	hdrTexture;
	if (data)
	{

		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}
	stbi_set_flip_vertically_on_load(false);

}

void Skybox::GenerateFramebuffers()
{
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	GL_RENDERBUFFER, captureRBO);

	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (size_t i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Skybox::GenerateCubemap(ShaderClass& shader, int ScrHeight, int ScrWidht)
{

	shader.use();
	shader.setInt("equirectangularMap", 0);
	shader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (size_t i = 0; i < 6; i++)
	{
		shader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		util::renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, ScrHeight, ScrWidht);


}

void Skybox::GenerateFramebuffersIrradiance()
{
	glGenFramebuffers(1, &captureFBOIrradiance);
	glGenRenderbuffers(1, &captureRBOIrradiance);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBOIrradiance);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBOIrradiance);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	GL_RENDERBUFFER, captureRBOIrradiance);

	glGenTextures(1, &irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
	for (size_t i = 0; i < 6; i++)
	{	

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0,
			GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void Skybox::GenerateIradianceMap(ShaderClass& shader, int ScrHeight, int ScrWidht)
{

	shader.use();
	shader.setInt("environmentMap", 0);
	shader.setMat4("projection", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBOIrradiance);
	for (size_t i = 0; i < 6; i++)
	{
		shader.setMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		util::renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, ScrHeight, ScrWidht);
}
