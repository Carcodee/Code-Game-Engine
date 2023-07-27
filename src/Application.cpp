#include <GL/glew.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "../Camera/CameraHandler.h"


#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "../ClassesOpengl/VAO.h"
#include "../ClassesOpengl/VBO.h"
#include "../ClassesOpengl/EBO.h"
#include "../Shaders/ShaderClass.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
using namespace glm;




//pointers examples

int sum(int& num1, int& num2) {

	return num1 + num2;
}

int sum(int* num1, int* num2) {

	return *num1 + *num2;
}

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// camera

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main(void)
{
	GLFWwindow* window;






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

	//IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window , true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;


	// configure global opengl state
// -----------------------------
	glEnable(GL_DEPTH_TEST);


	ShaderClass myShader("Shaders/Base.vert", "Shaders/Base.frag");


	GLfloat vertices[] = {
	    -0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f,1.0f,0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,0.0f,1.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,1.0f,0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,0.0f,1.0f, 0.0f, 1.0f
	};
	// index data
	//GLuint indices[] = {
	//	// front and back
	//	0, 3, 2,
	//	2, 1, 0,
	//	4, 5, 6,
	//	6, 7 ,4,
	//	// left and right
	//	11, 8, 9,
	//	9, 10, 11,
	//	12, 13, 14,
	//	14, 15, 12,
	//	// bottom and top
	//	16, 17, 18,
	//	18, 19, 16,
	//	20, 21, 22,
	//	22, 23, 20
	//};
	
	//vao class handler
	VAO vao;
	//vbo class handler
	VBO vbo;
	//ebo class handler
	// 
	//EBO ebo;
	

	//all is binded here
	//vao
	vao.Bind();


	//vbo settings
	vbo.Bind();
	vbo.BufferData( sizeof(vertices)/ sizeof(GLfloat), vertices);

	//ebo settings;
	//ebo.Bind();
	//ebo.BufferData(sizeof(indices), indices);


	//Pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glEnableVertexAttribArray(0);


	//colour
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)( sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	//Textures
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// load and create a texture 
	// -------------------------
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);


	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture


	int width, height, nrChannels;
	//this needs to be png in order to configurate the alpha channel
	unsigned char* data = stbi_load("C:/Users/carlo/Documents/ImagesProgramming/container.jpg", &width, &height, &nrChannels, 0);



	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}
	stbi_image_free(data);


	//Text2----------------------

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("C:/Users/carlo/Documents/ImagesProgramming/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}
	stbi_image_free(data);

	//set the uniform data



	//UI system---------------

	float speedRotation=0;
	float speedMultiplier = 1;
	float cubePosX = 0;
	float cubePosY = 0;
	float cubePosZ = -3.0f;

	//Camera


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
		processInput(window);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
	

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime()* speedRotation * speedMultiplier* glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(model, glm::vec3(cubePosX, cubePosY, cubePosZ));

		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		//view = glm::translate(view, glm::vec3(cubePosX, cubePosY, cubePosZ));

		view = camera.GetViewMatrix();

		glm::mat4 projection = glm::mat4(1.0f);;
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH/ (float)SCR_HEIGHT, 0.1f, 100.0f);


		myShader.setMat4("model", model);
		myShader.setMat4("view", view);
		myShader.setMat4("projection", projection);



		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		myShader.setInt("texture1", 0); // or with shader class
		myShader.setInt("texture2", 1); // or with shader class

		myShader.use();
		vao.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);





		glEnd();
		//IMGUI
		if (ImGui::TreeNode("Cube Speed"))
		{
			const float spacing = 4;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

			static int int_value = 1;
			ImGui::VSliderInt("##int", ImVec2(18, 160), &int_value, 1, 10);
			ImGui::SameLine();
			speedMultiplier = int_value;
			static float values[1] = { 0.0f };
			speedRotation = values[0];
			ImGui::PushID("set1");
			for (int i = 0; i < 1; i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
				ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));
				ImGui::VSliderFloat("##v", ImVec2(18, 160), &values[i], -1.0f, 1.0f, "");
				if (ImGui::IsItemActive() || ImGui::IsItemHovered())
					ImGui::SetTooltip("%.3f", values[i]);
				ImGui::PopStyleColor(4);
				ImGui::PopID();
			}
	
			ImGui::PopID();

			ImGui::SameLine();
			ImGui::PushID("set3");
			for (int i = 0; i < 1; i++)
			{
				if (i > 0) ImGui::SameLine();
				ImGui::PushID(i);
				ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
				ImGui::VSliderFloat("##v", ImVec2(40, 160), &values[i], -1.0f, 1.0f, "%.2f\nsec");
				ImGui::PopStyleVar();
				ImGui::PopID();
			}
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Cube Movement"))
		{
			const float spacing = 4;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

			static float _valueX = 0;
			ImGui::VSliderFloat("##int", ImVec2(18, 160), &_valueX, -10, 10);
			ImGui::SameLine();
			cubePosX = _valueX;

			static float _valueY = 0;
			ImGui::VSliderFloat("##int1", ImVec2(18, 160), &_valueY, -10, 10);
			ImGui::SameLine();
			cubePosY = _valueY;
			static float _valueZ = -3.0f;
			ImGui::VSliderFloat("##int2", ImVec2(18, 160), &_valueZ, -10, 10);
			ImGui::SameLine();
			cubePosZ = _valueZ;


			ImGui::PushID("set1");
		

	
			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::TreePop();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());




		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
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
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) { 
		camera.MovementSpeed = 15.0f;
	}
	else
	{
		camera.MovementSpeed = 2.5f;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		camera.SetMouseSpeed(true);


	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		camera.SetMouseSpeed(false);

	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


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


