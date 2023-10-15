#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../Shaders/Classes/ShaderClass.h"
#include "../src/Functions/Utility.h"
#include <vector>

class Skybox
{
	public :
		Skybox(const char* filename);
		
		void GenerateFramebuffers();
		void GenerateCubemap(ShaderClass& shader, int ScrHeight, int ScrWidht);
		void GenerateFramebuffersIrradiance();
		void GenerateIradianceMap(ShaderClass& shader, int ScrHeight, int ScrWidht);
		unsigned int envCubemap;
		unsigned int irradianceMap;

private: 
	unsigned int hdrTexture;
	unsigned int captureFBO;
	unsigned int captureRBO;

	unsigned int captureFBOIrradiance;
	unsigned int captureRBOIrradiance;
	std::vector	<glm::mat4> captureViews;
	glm::mat4 captureProjection;
};

