#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include"../ClassesOpengl/ImguiSettings/ImguiRender.h"
#include "../ClassesOpengl/ImguiSettings/MyImguiSettings.h"
#include "headers/headers.h"
#include "../Functions/Utility.h"



using namespace glm;




GLFWwindow* window;

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 600;

bool bloomKeyPressed = true;
float exposure = 1.0f;
bool dirLightOn = true, pointLightOn = true, spotLightOn = true, HDR = false,bloom = false, useGbuffer = false, shadows = true;
bool flipUVS= false;
float heightScaleFactor = 0.1f;
bool PBR = false;
int modelsLoadedCounter = 0;
float roughnessM = 1.0f;
float metallicM = 1.0f;
float aoM = 1.0f;
float albedoM = 1.0f;


void DropCallback(GLFWwindow* window, int count, const char** paths);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


ModelHandler modelHandler;
std::string path;


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
MousePos mousePos;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

#ifdef _DEBUG
#define PRINTL(x) std::cout<< x << "\n"
#define PRINT(x) std::cout<<x
#else
#define PRINTL(x)
#define PRINT(x)
#endif
int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CUBEUI", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	//Create context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	ImguiRender myImgui(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;


	glfwSetDropCallback(window, DropCallback);
	// configure global opengl state
// -----------------------------
	glEnable(GL_DEPTH_TEST);


	ShaderClass myShader("Shaders/Base.vert", "Shaders/Base.frag");
	ShaderClass lightShader("Shaders/Light.vert","Shaders/Light.frag");
	ShaderClass cubemapShader("Shaders/Cubemap.vert", "Shaders/Cubemap.frag");
	ShaderClass fbShader("Shaders/framebuffer.vert", "Shaders/Framebuffer.frag");
	ShaderClass modelShader("Shaders/ModelLoader.vert", "Shaders/ModelLoader.frag");

	ShaderClass fbBlurShader("Shaders/Blur.vert", "Shaders/Blur.frag");
	ShaderClass finalBloomShader("Shaders/FinalBloom.vert", "Shaders/FinalBloom.frag");
	ShaderClass geometricBufferShader("Shaders/GBuffer.vert", "Shaders/GBuffer.frag");
	ShaderClass gBufferLightPassShader("Shaders/LightPass.vert", "Shaders/LightPass.frag");
	ShaderClass shadowShaderDepth("Shaders/ShadowMap.vert", "Shaders/ShadowMap.frag");
	ShaderClass pickingShader("Shaders/Picking.vert", "Shaders/Picking.frag");

	std::vector <GLfloat> cubeLightVertices = util::returnVertices(util::cube3Layout);
	std::vector <GLfloat> vertices = util::returnVertices(util::cube4Layout);
	 //index data
	std::vector <GLuint> indices = {
		// front and back
		0, 3, 2,
		2, 1, 0,
		4, 5, 6,
		6, 7 ,4,
		// left and right
		11, 8, 9,
		9, 10, 11,
		12, 13, 14,
		14, 15, 12,
		// bottom and top
		16, 17, 18,
		18, 19, 16,
		20, 21, 22,
		22, 23, 20
	};





	//vao class handler
	VAO vao;
	//vbo class handler
	VBO vbo;
	//ebo class handler
	// 
	EBO ebo;
	

	//all is binded here
	//vao
	vao.Bind();


	//vbo settings
	vbo.Bind();
	vbo.BufferData(vertices.size(),vertices.data());

	//ebo settings;
	ebo.Bind();
	ebo.BufferData(indices.size(), indices.data());


	//Pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)0);
	glEnableVertexAttribArray(0);


	//colour
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	//Textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 11, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	//normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float)* 11 , (void*)(sizeof(float)*8));
	glEnableVertexAttribArray(3);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// load and create a texture 
	// -------------------------
	unsigned int albedo = util::LoadTexture("Models/rust/albedo.png");
	unsigned int normal = util::LoadTexture("Models/rust/normal.png");
	unsigned int metallic = util::LoadTexture("Models/rust/metallic.png");
	unsigned int roughness = util::LoadTexture("Models/rust/roughness.png");
	unsigned int ao = util::LoadTexture("Models/rust/ao.png");
	
	myShader.use();
	myShader.setInt("albedoMap", 0);
	myShader.setInt("normalMap", 1);
	myShader.setInt("metallicMap", 2);
	myShader.setInt("roughnessMap", 3);
	myShader.setInt("aoMap", 4);
	//Cubemap

	std::vector <GLfloat> skyboxVertices = util::returnVertices(util::cube1Layout);

	VAO skyboxVAO;
	skyboxVAO.Bind();

	VBO skyboxVBO;
	skyboxVBO.Bind();

	skyboxVBO.BufferData(skyboxVertices.size(), skyboxVertices.data());
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	std::vector<std::string> textFaces
	{
			"C:/Users/carlo/OneDrive/Documents/GitHub/OpenglCarlos/ImagesProgramming/skybox/right.jpg",
			"C:/Users/carlo/OneDrive/Documents/GitHub/OpenglCarlos/ImagesProgramming/skybox/left.jpg",
			"C:/Users/carlo/OneDrive/Documents/GitHub/OpenglCarlos/ImagesProgramming/skybox/top.jpg",
			"C:/Users/carlo/OneDrive/Documents/GitHub/OpenglCarlos/ImagesProgramming/skybox/bottom.jpg",
			"C:/Users/carlo/OneDrive/Documents/GitHub/OpenglCarlos/ImagesProgramming/skybox/front.jpg",
			"C:/Users/carlo/OneDrive/Documents/GitHub/OpenglCarlos/ImagesProgramming/skybox/back.jpg"
	};

	unsigned int textureCubeMap=util::LoadCubemap(textFaces);

	cubemapShader.use();
	cubemapShader.setInt("skybox", 0);
	skyboxVAO.UnBind();

	//UI system---------------

	float speedRotation=0;
	float speedMultiplier = 1;
	float cubePosX = 0;
	float cubePosY = 0;
	float cubePosZ = -3.0f;
	float noiseMult = 0;


	//light

	glm::vec3 lightCol (1);
	float lightX = 0;
	float lightY = 0;
	float lightZ = -3.0f;


	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO;
	lightVBO.Bind();
	lightVBO.BufferData(cubeLightVertices.size(),cubeLightVertices.data());

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	lightVAO.UnBind();


#pragma region ShadowFramebuffer

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


#pragma endregion


#pragma region basic Fbo

		unsigned int fbo;
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		unsigned int textureColorbuffer;
		glGenTextures(1, &textureColorbuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

		//render buffer obj
		unsigned int rbo;
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		fbShader.setInt("screenTexture", 0);
	
	
#pragma endregion


#pragma region Bloom


		unsigned int hdrFBO;
		glGenFramebuffers(1, &hdrFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		// create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
		unsigned int colorBuffers[2];
		glGenTextures(2, colorBuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			// attach texture to framebuffer
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
		}
		// create and attach depth buffer (renderbuffer)
		unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, attachments);
		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// ping-pong-framebuffer for blurring
		unsigned int pingpongFBO[2];
		unsigned int pingpongColorbuffers[2];
		glGenFramebuffers(2, pingpongFBO);
		glGenTextures(2, pingpongColorbuffers);
		for (unsigned int i = 0; i < 2; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
			glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
			// also check if framebuffers are complete (no need for depth buffer)
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "Framebuffer not complete!" << std::endl;

		}

		fbBlurShader.use();
		fbBlurShader.setInt("image", 0);
		finalBloomShader.use();
		finalBloomShader.setInt("scene", 0);
		finalBloomShader.setInt("bloomBlur", 1);
	






#pragma endregion
	
		Framebuffer finalFbo;
		finalFbo.generateTexture(window);
		fbShader.setInt("screenTexture", 0);

		Framebuffer pickingFbo;
		pickingFbo.generateTexture(window);


	//Color
	glm::vec3 Color(1);


	//intances


	int instanceID = 0;
	int amount = 2;

	//Noise
	PerlinNoise myPNoise;
	double noiseX;

	//random val
	glm::vec3* randomCols;
	randomCols = new glm::vec3[amount*amount];;
	for (size_t i = 0; i < amount * amount; i++)
	{
		randomCols[i] = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
	}
	//Shadows


	//PBR
	glm::vec3 lightPositions[] = {
	glm::vec3(0.0f, 0.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(150.0f, 150.0f, 150.0f),
	};





	ImGuiIO& io = ImGui::GetIO();
	ImFont* font1 = io.Fonts->AddFontFromFileTTF("Fonts/Roboto-MediumItalic.ttf", 13.0f);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		processInput(window);

		myImgui.NewFrame();

		ImGui::PushFont(font1);

		/* Render here */
		// render
		// ------

		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		//glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		// make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region configurations

		//isntancesPos
		glm::mat4 modelMoved = glm::mat4(1.0f);
		modelMoved = glm::rotate(modelMoved, (float)glfwGetTime() * speedRotation * speedMultiplier * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(modelMoved, glm::vec3(cubePosX, cubePosY, cubePosZ));
		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		//view = glm::translate(view, glm::vec3(cubePosX, cubePosY, cubePosZ));
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);;
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		glm::mat4 projectionM = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewM = camera.GetViewMatrix();
		glm::vec3 lightPos(lightX, lightY, lightZ);

		float near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		//
		glm::mat4 lightView = glm::lookAt(lightPos,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
		 
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		// 
		glm::vec3 lightColor;
		lightColor.x = sin(2.0f);
		lightColor.y = sin(0.7f);
		lightColor.z = sin(1.3f);

		glm::vec3 diffuseColor = lightColor * 1.0f;
		glm::vec3 ambientColor = glm::vec3(0.1f);
		glm::mat4* modelMatrices;

#pragma endregion

#pragma region Picking ObjectRegion

		glViewport(0, 0, myImgui.viewportWindowSize.x, myImgui.viewportWindowSize.y);
		pickingFbo.Bind();
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pickingShader.use();

		for (size_t i = 0; i < modelHandler.models.size(); i++)
		{
			int r = (i & 0x000000FF) >> 0;
			int g = (i & 0x0000FF00) >> 8;
			int b = (i & 0x00FF0000) >> 16;
			glm::vec3 color(r/255.0f, g/255.0f, b/255.0f);

			pickingShader.setVec3("UniqueColor", color);
			modelHandler.DrawModel(pickingShader, i, projectionM, viewM);

		}
		glFlush();
		glFinish();

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		if (mousePos.isPressed)
		{

			unsigned char data[4];

			glReadPixels(mousePos.xpos, mousePos.ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
			std::cout<< mousePos.xpos <<" "<< mousePos.ypos <<std::endl;
			std::cout << "R: " << (int)data[0] << " G: " << (int)data[1] << " B: " << (int)data[2] << std::endl;
			int pickedID =
				data[0] +
				data[1] * 256 +
				data[2] * 256 * 256;
			if (pickedID<=modelHandler.models.size())
			{
				modelHandler.SetModelPicked(pickedID);
			}
			std::cout << "pickedID: " << pickedID << std::endl;
		}

		pickingFbo.UnBind();


#pragma endregion

#pragma region ShadowsRegion

		if (shadows)
		{
			shadowShaderDepth.use();
			shadowShaderDepth.setMat4("lightSpaceMatrix", lightSpaceMatrix);
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, albedo);

			shadowShaderDepth.use();

			// bind textures on corresponding texture units
			//instances
			modelMatrices = new glm::mat4[amount * amount];

			for (size_t k = 0; k < 10; k++)
			{
				for (size_t i = 0; i < amount; i++)
				{
					for (size_t j = 0; j < amount; j++)
					{
						glm::mat4 model = glm::mat4(1.0f);

						noiseX = { myPNoise.noise(i + cubePosX,
							k + cubePosY ,
							j + cubePosZ) * (10 * sin((float)glfwGetTime()) * noiseMult) };

						model = glm::translate(model, glm::vec3(i + cubePosX, (k + cubePosY) + noiseX, j + cubePosZ));
						// 4. now add to list of matrices
						model = glm::rotate(model, (float)glfwGetTime() * speedRotation * speedMultiplier * glm::sin(glm::radians(50.0f)), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(model, glm::vec3(cubePosX, cubePosY, cubePosZ));;
						shadowShaderDepth.use();
						modelMatrices[instanceID] = model;
						shadowShaderDepth.setMat4("model[" + std::to_string(instanceID) + "]", model);
						instanceID++;
					}
				}
				instanceID = 0;
				vao.Bind();
				glDrawArraysInstanced(GL_TRIANGLES, 0, 36, amount * amount);
			}
			glBindVertexArray(0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}


#pragma endregion



		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		(bloom) ? glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO) : glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#pragma region light



		//lightPos

		lightShader.use();
		// view/projection transformations

		lightShader.setMat4("projection", projectionM);
		lightShader.setMat4("view", viewM);
		// world transformation
		glm::mat4 lightModel = glm::mat4(1.0f);
	    lightModel=glm::translate(lightModel, lightPos);
		lightShader.setMat4("model", lightModel);


		// be sure to activate shader when setting uniforms/drawing objects
		lightShader.setVec3("objectColor", glm::vec3(1.0));
		// render the cube
		lightVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

#pragma endregion

#pragma region CubesRegion

		myShader.use();

		// bind textures on corresponding texture units

		myShader.setInt("material.diffuse", 0);
		myShader.setInt("material.specular", 1);
		myShader.setFloat("material.shininess", 256.0f);
		myShader.setFloat("roughnessM", roughnessM);
		myShader.setFloat("metallicM", metallicM);
		myShader.setFloat("aoM", aoM);
		myShader.setFloat("albedoM", albedoM);

		myShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallic);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughness);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, ao);
		glBindTexture(GL_TEXTURE2, depthMap);

		myShader.setVec3("Color", Color);
		myShader.setVec3("viewPos", camera.Position);
		myShader.setMat4("view", viewM);
		myShader.setMat4("projection", projectionM);

		util::HandleLights(dirLightOn, pointLightOn, myShader, lightPos, ambientColor, diffuseColor);

		(shadows) ? myShader.setBool("shadowsOn", shadows) : myShader.setBool("shadowsOn", !shadows);
		

	
		//instances
		modelMatrices = new glm::mat4[amount * amount];

		for (size_t k = 0; k < 10; k++)
		{
			for (size_t i = 0; i < amount; i++)
			{
				for (size_t j = 0; j < amount; j++)
				{
					glm::mat4 model = glm::mat4(1.0f);

					noiseX = { myPNoise.noise(i + cubePosX,
						k + cubePosY ,
						j + cubePosZ) * (10 * sin((float)glfwGetTime()) * noiseMult) };

					model = glm::translate(model, glm::vec3(i + cubePosX, (k + cubePosY) + noiseX, j + cubePosZ));
					// 4. now add to list of matrices
					model = glm::rotate(model, (float)glfwGetTime() * speedRotation * speedMultiplier * glm::sin(glm::radians(50.0f)), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(model, glm::vec3(cubePosX, cubePosY, cubePosZ));;
					myShader.use();
					modelMatrices[instanceID] = model;
					myShader.setVec3("lightPositions[" + std::to_string(i) + "]", lightPos);
					myShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[0]);
					myShader.setMat4("model[" + std::to_string(instanceID) + "]", model);
					instanceID++;
				}
			}
			instanceID = 0;
			vao.Bind();
			glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size(), amount* amount);

			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, amount * amount);
		}
		//glDrawElementsInstanced(GL_TRIANGLES,36,GL_UNSIGNED_INT,0,amount);
		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices.data());
		glBindVertexArray(0);

#pragma endregion

#pragma region Cubemap

		glDepthFunc(GL_LEQUAL);
		cubemapShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

		cubemapShader.setMat4("view", view);
		cubemapShader.setMat4("projection", projection);
		skyboxVAO.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMap);

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMap);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

#pragma endregion


#pragma region ModelRegion

		for (size_t i = 0; i < modelHandler.models.size(); i++)
		{
			if (modelHandler.models[i].newModel.isLoaded)
			{
				// don't forget to enable shader before setting uniforms
				modelShader.use();
				modelShader.setVec3("viewPos", camera.Position);
				modelShader.setVec3("lightPos", -lightPos);
				modelShader.setVec3("dirLight.ambient", ambientColor);
				modelShader.setVec3("dirLight.diffuse", glm::vec3(0.3f,0.8f,0.8f));
				modelShader.setVec3("dirLight.specular", glm::vec3(.5f));
				modelShader.setVec3("dirLight.direction", -lightPos);


			}
			if (flipUVS)
			{
				stbi_set_flip_vertically_on_load(true);
				modelHandler.DrawModel(modelShader, i,projectionM,viewM);
				stbi_set_flip_vertically_on_load(false);
			}
			else
			{
				modelHandler.DrawModel(modelShader, i, projectionM, viewM);

			}
		}
		


#pragma endregion

#pragma region frambebuffersTest

		if (!bloom)
		{	
			// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST | GL_DEPTH_BUFFER_BIT); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);
		finalFbo.Bind();

		fbShader.use();
		fbShader.setBool("HDR", HDR);

		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		util::renderQuad();
		finalFbo.UnBind();


		}



#pragma endregion

#pragma region Bloom render

		if (bloom)
		{
			// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

			bool horizontal = true, first_iteration = true;
			unsigned int amountIter = 10;
			fbBlurShader.use();
			for (unsigned int i = 0; i < amountIter; i++)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
				glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
				fbBlurShader.setInt("horizontal", horizontal);

				util::renderQuad();

				horizontal = !horizontal;
				if (first_iteration)
					first_iteration = false;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			finalFbo.Bind();

			glDisable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			finalBloomShader.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
			finalBloomShader.setInt("bloom", bloom);
			finalBloomShader.setFloat("exposure", exposure);
			finalBloomShader.setBool("hdr", HDR);
			util::renderQuad();

			finalFbo.UnBind();

			//std::cout << "bloom: " << (bloom ? "on" : "off") << "| exposure: " << exposure << std::endl;
			//clear all relevant buffers


		}



#pragma endregion


		glEnd();


#pragma region imgui

		myImgui.CreateNode([&]() {MoveCubes(speedMultiplier, speedRotation); });
		myImgui.CreateNode([&]() {cubes_Movement(cubePosX, cubePosY, cubePosZ, noiseMult); });
		myImgui.CreateNode([&]() {cube_Amount(amount, roughnessM, metallicM, aoM, albedoM); });
		myImgui.CreateNode([&]() {light_Movement(lightX, lightY, lightZ); });
		myImgui.CreateNode([&]() {height_Mapping(heightScaleFactor); });
		myImgui.CreateNode([&]() {light_Settings(dirLightOn, spotLightOn, pointLightOn, HDR, bloom, shadows); });
		myImgui.CreateNode([&]() {model_LoaderTest(modelHandler, flipUVS, modelsLoadedCounter,path); });
		myImgui.CreateNode([&]() {model_configs(modelHandler); });

		ImGui::PopFont();


		ImGui::ShowDemoWindow();
		
		myImgui.CreateViewPort(finalFbo.m_Texture, modelHandler);
		myImgui.SetGizmoOperation(window);

		myImgui.Render();
#pragma endregion

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

#pragma region  callbacks

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		camera.SetMouseSpeed(true);
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			camera.ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.ProcessKeyboard(DOWN, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			camera.MovementSpeed = 35.0f;
		}
		else
		{
			camera.MovementSpeed = 5.0f;
		}

	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		camera.SetMouseSpeed(false);

	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		
		glfwGetCursorPos(window, &mousePos.xpos, &mousePos.ypos);
		mousePos.isPressed=true;

	}
	else
	{

		mousePos.isPressed = false;
	}

}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
void DropCallback(GLFWwindow* window, int count, const char** paths)
{
	for (int i = 0; i < count; ++i)
	{
		std::cout << paths[i] << std::endl;
		path= paths[i];
	}
	std::shared_ptr<Material> material = std::make_shared<Material>();
	ModelItem model = { Model(),modelsLoadedCounter ,glm::mat4(1.0f), "Entity ", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.0f, "Base", material };
	modelHandler.models.push_back(model);
	modelHandler.models[modelsLoadedCounter].newModel.StartModel(path, PBR, material);
	modelsLoadedCounter++;

}
#pragma endregion
