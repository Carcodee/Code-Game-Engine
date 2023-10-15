#include "Utility.h"


	unsigned int util::LoadTexture(char const* path) {

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}

	std::vector<GLfloat> util::returnVertices(vertexTypes type)
	{

		switch (type)
		{
		case quad:
			return std::vector<GLfloat> { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
				// positions   // texCoords
				-1.0f, 1.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f,
					1.0f, -1.0f, 1.0f, 0.0f,

					-1.0f, 1.0f, 0.0f, 1.0f,
					1.0f, -1.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f
			};
			break;

		case cube1Layout:
			return std::vector<GLfloat>{
				// positions          
				-1.0f, 1.0f, -1.0f,
					-1.0f, -1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, -1.0f,

					-1.0f, -1.0f, 1.0f,
					-1.0f, -1.0f, -1.0f,
					-1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, -1.0f,
					-1.0f, 1.0f, 1.0f,
					-1.0f, -1.0f, 1.0f,

					1.0f, -1.0f, -1.0f,
					1.0f, -1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,

					-1.0f, -1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, -1.0f, 1.0f,
					-1.0f, -1.0f, 1.0f,

					-1.0f, 1.0f, -1.0f,
					1.0f, 1.0f, -1.0f,
					1.0f, 1.0f, 1.0f,
					1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f, 1.0f,
					-1.0f, 1.0f, -1.0f,

					-1.0f, -1.0f, -1.0f,
					-1.0f, -1.0f, 1.0f,
					1.0f, -1.0f, -1.0f,
					1.0f, -1.0f, -1.0f,
					-1.0f, -1.0f, 1.0f,
					1.0f, -1.0f, 1.0f
			};
		case cube3Layout:
			return std::vector<GLfloat> {
			// back face
				-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
					1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
					1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
					1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
					-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
					-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top-left
					// front face
					-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
					1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // bottom-right
					1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
					1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
					-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // top-left
					-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
					// left face
					-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
					-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
					-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
					-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
					-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
					-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
					// right face
					1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
					1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
					1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
					1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
					1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
					1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
					// bottom face
					-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
					1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
					1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
					1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
					-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
					-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
					// top face
					-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
					1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
					1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
					1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
					-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
					-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f  // bottom-left        
		};
		case cube4Layout:
				return std::vector<GLfloat> {
				// positions          // colour     // texture       //normal 
				-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
				-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
				-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

				-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

				-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
				-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

				-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
				-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
				-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

				-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		};
			break;	


			default:
				return std::vector<GLfloat> { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
					// positions   // texCoords
					-1.0f, 1.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f,
					1.0f, -1.0f, 1.0f, 0.0f,

					-1.0f, 1.0f, 0.0f, 1.0f,
					1.0f, -1.0f, 1.0f, 0.0f,
					1.0f, 1.0f, 1.0f, 1.0f
				};
			break;
		}

		return std::vector<float>();
	}

	unsigned int util::LoadCubemap(std::vector <std::string> faces) {
		unsigned int textureCubeMap;
		glGenTextures(1, &textureCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMap);
		int width, height, nrChannels;
		for (unsigned int i = 0; i < faces.size(); i++)
		{
			//std::string file = "C:/Users/carlo/Documents/ImagesProgramming/skybox/" + textFaces[i];
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		return textureCubeMap;
	}
	void util::HandleLights(bool& dirLight, bool& pointLights, ShaderClass& myShader, glm::vec3& lightPos, glm::vec3& ambientColor, glm::vec3& diffuseColor) {

		glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
		};
		if (dirLight)
		{
			myShader.setVec3("dirLight.direction", -lightPos);
			myShader.setVec3("dirLight.ambient", ambientColor);
			myShader.setVec3("dirLight.diffuse", diffuseColor);
			myShader.setVec3("dirLight.specular", glm::vec3(.5f));
		}
		else
		{
			myShader.setVec3("dirLight.direction", glm::vec3(0));
			myShader.setVec3("dirLight.ambient", glm::vec3(0));
			myShader.setVec3("dirLight.diffuse", glm::vec3(0));
			myShader.setVec3("dirLight.specular", glm::vec3(.5f));
		}
		if (pointLights)
		{
			myShader.setVec3("pointLights[0].position", pointLightPositions[0]);
			myShader.setVec3("pointLights[0].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			myShader.setVec3("pointLights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			myShader.setVec3("pointLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			myShader.setFloat("pointLights[0].constant", 1.0f);
			myShader.setFloat("pointLights[0].linear", 0.09f);
			myShader.setFloat("pointLights[0].quadratic", 0.032f);

			myShader.setVec3("pointLights[1].position", pointLightPositions[1]);
			myShader.setVec3("pointLights[1].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			myShader.setVec3("pointLights[1].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			myShader.setVec3("pointLights[1].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			myShader.setFloat("pointLights[1].constant", 1.0f);
			myShader.setFloat("pointLights[1].linear", 0.09f);
			myShader.setFloat("pointLights[1].quadratic", 0.032f);

			myShader.setVec3("pointLights[2].position", pointLightPositions[2]);
			myShader.setVec3("pointLights[2].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			myShader.setVec3("pointLights[2].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			myShader.setVec3("pointLights[2].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			myShader.setFloat("pointLights[2].constant", 1.0f);
			myShader.setFloat("pointLights[2].linear", 0.09f);
			myShader.setFloat("pointLights[2].quadratic", 0.032f);

			myShader.setVec3("pointLights[3].position", pointLightPositions[3]);
			myShader.setVec3("pointLights[3].ambient", glm::vec3(0.05f, 0.05f, 0.05f));
			myShader.setVec3("pointLights[3].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
			myShader.setVec3("pointLights[3].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			myShader.setFloat("pointLights[3].constant", 1.0f);
			myShader.setFloat("pointLights[3].linear", 0.09f);
			myShader.setFloat("pointLights[3].quadratic", 0.032f);
		}
		else
		{
			myShader.setVec3("pointLights[0].position", pointLightPositions[0]);
			myShader.setVec3("pointLights[0].ambient", glm::vec3(0));
			myShader.setVec3("pointLights[0].diffuse", glm::vec3(0));
			myShader.setVec3("pointLights[0].specular", glm::vec3(0));
			myShader.setFloat("pointLights[0].constant", 1.0f);
			myShader.setFloat("pointLights[0].linear", 0.09f);
			myShader.setFloat("pointLights[0].quadratic", 0.032f);

			myShader.setVec3("pointLights[1].position", pointLightPositions[1]);
			myShader.setVec3("pointLights[1].ambient", glm::vec3(0));
			myShader.setVec3("pointLights[1].diffuse", glm::vec3(0));
			myShader.setVec3("pointLights[1].specular", glm::vec3(0));
			myShader.setFloat("pointLights[1].constant", 1.0f);
			myShader.setFloat("pointLights[1].linear", 0.09f);
			myShader.setFloat("pointLights[1].quadratic", 0.032f);

			myShader.setVec3("pointLights[2].position", pointLightPositions[2]);
			myShader.setVec3("pointLights[2].ambient", glm::vec3(0));
			myShader.setVec3("pointLights[2].diffuse", glm::vec3(0));
			myShader.setVec3("pointLights[2].specular", glm::vec3(0));
			myShader.setFloat("pointLights[2].constant", 1.0f);
			myShader.setFloat("pointLights[2].linear", 0.09f);
			myShader.setFloat("pointLights[2].quadratic", 0.032f);

			myShader.setVec3("pointLights[3].position", pointLightPositions[3]);
			myShader.setVec3("pointLights[3].ambient", glm::vec3(0));
			myShader.setVec3("pointLights[3].diffuse", glm::vec3(0));
			myShader.setVec3("pointLights[3].specular", glm::vec3(0));
			myShader.setFloat("pointLights[3].constant", 1.0f);
			myShader.setFloat("pointLights[3].linear", 0.09f);
			myShader.setFloat("pointLights[3].quadratic", 0.032f);
		}

	}
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	void util::renderCube()
	{
		// initialize (if necessary)
		if (cubeVAO == 0)
		{
			std::vector<GLfloat> vertices = returnVertices(cube3Layout);
			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &cubeVBO);
			// fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(cubeVAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		// render Cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}


	unsigned int quadVAO = 0;
	unsigned int quadVBO = 0;
	void util::renderQuad()
	{
		if (quadVAO == 0) {
			
			float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
				// positions   // texCoords
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
			};

			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}



