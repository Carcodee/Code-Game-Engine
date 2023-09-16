#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "../Shaders/ShaderClass.h"

#include "stb-master/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION


namespace util {
	enum vertexTypes {
		quad,
		cube1Layout,
		cube2Layout,
		cube3Layout,
		cube4Layout,
		cube5Layout,
		cube6Layout,
	};

	void renderCube();
	void renderQuad();
	void HandleLights(bool& dirLight, bool& pointLights, ShaderClass& myShader, glm::vec3& lightPos, glm::vec3& ambientColor, glm::vec3& diffuseColor);
	unsigned int LoadCubemap(std::vector <std::string> faces);
	unsigned int LoadTexture(char const* path);
	std::vector<GLfloat> returnVertices(vertexTypes type);
}
