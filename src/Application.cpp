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
#include "../Mesh/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include "Noise/PerlinNoise.h"
#include "OBJLoader/OBJ_Loader.h"
#include "../Model/Model.h"
#include "../Mesh/Mesh.h"
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

bool bloomKeyPressed = true;
float exposure = 1.0f;
bool dirLightOn = true, pointLightOn = true, spotLightOn = true, HDR = true,bloom = true;

// camera

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int LoadCubemap(std::vector <std::string> faces);
unsigned int LoadTexture(char const* path);
void HandleLights(bool& dirLight, bool& pointLights, ShaderClass& myShader, glm::vec3& lightPos, glm::vec3& ambientColor, glm::vec3& diffuseColor);

void renderQuad();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controlbs
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;


	// configure global opengl state
// -----------------------------
	glEnable(GL_DEPTH_TEST);


	ShaderClass myShader("Shaders/Base.vert", "Shaders/Base.frag");
	ShaderClass lightShader("Shaders/Light.vert","Shaders/Light.frag");
	ShaderClass cubemapShader("Shaders/Cubemap.vert", "Shaders/Cubemap.frag");
	ShaderClass modelShader("Shaders/ModelLoader.vert", "Shaders/ModelLoader.frag");
	ShaderClass fbShader("Shaders/framebuffer.vert", "Shaders/Framebuffer.frag");

	ShaderClass fbBlurShader("Shaders/Blur.vert", "Shaders/Blur.frag");
	ShaderClass finalBloomShader("Shaders/FinalBloom.vert", "Shaders/FinalBloom.frag");


	stbi_set_flip_vertically_on_load(true);
	
	Model ourModel("Models/BackpackModel/backpack.obj");
	stbi_set_flip_vertically_on_load(false);

	//Model loader
	//objl::Loader myLoader;
	//std::vector <GLfloat> vertices;
	//std::vector <GLuint> indices;

	//if (myLoader.LoadFile("Models/AirBoat.obj"))
	//{
	//	objl::Mesh myMesh = myLoader.LoadedMeshes[0];

	//		for (size_t i = 0; i < myMesh.Vertices.size(); i++)
	//		{
	//			objl::Vertex v = myMesh.Vertices[i];
	//			v.Position;
	//			//color
	//			v.TextureCoordinate;
	//			v.Normal;
	//			//pos
	//			vertices.push_back(v.Position.X);
	//			vertices.push_back(v.Position.Y);
	//			vertices.push_back(v.Position.Z);
	//			//col
	//			vertices.push_back(1.0f);
	//			vertices.push_back(1.0f);
	//			vertices.push_back(1.0f);
	//			//text
	//			vertices.push_back(v.TextureCoordinate.X);
	//			vertices.push_back(v.TextureCoordinate.Y);
	//			//norm
	//			vertices.push_back(v.Normal.X);
	//			vertices.push_back(v.Normal.Y);
	//			vertices.push_back(v.Normal.Z);

	//			//std::cout << v.Position.X<<" ";
	//			//std::cout << v.Position.Y<<" ";
	//			//std::cout << v.Position.Z<<" ";
	//			//std::cout << (1.0f) << " ";
	//			//std::cout << (1.0f) << " ";
	//			//std::cout << (1.0f) << " ";
	//			//std::cout << v.TextureCoordinate.X << " ";
	//			//std::cout << v.TextureCoordinate.Y << " ";
	//			//std::cout << v.Normal.X << " ";
	//			//std::cout << v.Normal.Y << " ";
	//			//std::cout << v.Normal.Z << "\n";

	//		}
	//	


	//	indices = myMesh.Indices;

	//}



	std::vector <GLfloat> cubeLightVertices = {
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			 // bottom face
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 // top face
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left   

	
	};

	std::vector <GLfloat> vertices = {
	    -0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f, 0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f,0.0f,0.0f, 0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f, 1.0f,0.0f,0.0f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f,1.0f,0.0f, 1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,0.0f,1.0f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,1.0f,0.0f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,0.0f,1.0f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	};
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
	unsigned int texture1, specularMap;
	texture1= LoadTexture("C:/Users/carlo/OneDrive/Documents/GitHub/OpenglCarlos/ImagesProgramming/container2.png");
	specularMap = LoadTexture("C:/Users/carlo/OneDrive/Documents/GitHub/OpenglCarlos/ImagesProgramming/lighting_maps_specular_color.png");

	

	//Cubemap

	std::vector <GLfloat> skyboxVertices = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

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

	unsigned int textureCubeMap=LoadCubemap(textFaces);

	cubemapShader.use();
	cubemapShader.setInt("skybox", 0);
	skyboxVAO.UnBind();

	//UI system---------------

	float speedRotation=0;
	float speedMultiplier = 1;
	float cubePosX = 0;
	float cubePosY = 0;
	float cubePosZ = -3.0f;

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
	//new object

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
		// render
		// ------
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		(bloom) ? glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO) : glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// make sure we clear the framebuffer's content
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



#pragma region configurations

		myShader.use();
		//isntancesPos
		glm::mat4 modelMoved = glm::mat4(1.0f);
		modelMoved = glm::rotate(modelMoved, (float)glfwGetTime() * speedRotation * speedMultiplier * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(modelMoved, glm::vec3(cubePosX, cubePosY, cubePosZ));
		glm::mat4 view = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		//view = glm::translate(view, glm::vec3(cubePosX, cubePosY, cubePosZ));
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::mat4(1.0f);;
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		myShader.setMat4("view", view);
		myShader.setMat4("projection", projection);

#pragma endregion

#pragma region light



		//lightPos
		glm::vec3 lightPos(lightX, lightY, lightZ);

		lightShader.use();
		// view/projection transformations
		glm::mat4 lightProjection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 lightView = camera.GetViewMatrix();
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		// world transformation
		glm::mat4 lightModel = glm::mat4(1.0f);
	    lightModel=glm::translate(lightModel, lightPos);
		lightShader.setMat4("model", lightModel);

		// 
		glm::vec3 lightColor;
		lightColor.x = sin(2.0f);
		lightColor.y = sin(0.7f);
		lightColor.z = sin(1.3f);

		glm::vec3 diffuseColor = lightColor * 1.0f;
		glm::vec3 ambientColor = glm::vec3(0.1f);
		// be sure to activate shader when setting uniforms/drawing objects
		lightShader.setVec3("objectColor", glm::vec3(1.0));
		// render the cube
		lightVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

#pragma endregion

#pragma region ModelRegion

		// don't forget to enable shader before setting uniforms
		modelShader.use();
		modelShader.setVec3("viewPos", camera.Position);
		modelShader.setVec3("dirLight.direction", -lightPos);
		modelShader.setVec3("dirLight.ambient", ambientColor);
		modelShader.setVec3("dirLight.diffuse", glm::vec3(1.0f));
		modelShader.setVec3("dirLight.specular", glm::vec3(.5f));
		// view/projection transformations
		glm::mat4 projectionM = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewM = camera.GetViewMatrix();
		modelShader.setMat4("projection", projectionM);
		modelShader.setMat4("view", viewM);

		// render the loaded model
		glm::mat4 modelM = glm::mat4(1.0f);
		modelM = glm::translate(modelM, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		modelM = glm::scale(modelM, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		modelShader.setMat4("model", modelM);
		ourModel.Draw(modelShader);

#pragma endregion

#pragma region CubesRegion

		myShader.use();

		// bind textures on corresponding texture units

		myShader.setInt("material.diffuse", 0);
		myShader.setInt("material.specular", 1);
		myShader.setFloat("material.shininess", 256.0f);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		myShader.setVec3("Color", Color);
		myShader.setVec3("viewPos", camera.Position);

		HandleLights(dirLightOn, pointLightOn, myShader, lightPos, ambientColor, diffuseColor);

		//instances
		glm::mat4* modelMatrices;
		modelMatrices = new glm::mat4[amount * amount];

		for (size_t k = 0; k < 1; k++)
		{
			for (size_t i = 0; i < amount; i++)
			{
				for (size_t j = 0; j < amount; j++)
				{
					glm::mat4 model = glm::mat4(1.0f);

					noiseX = { myPNoise.noise(i + cubePosX,
						k + cubePosY * 10 * sin((float)glfwGetTime()) * speedMultiplier,
						j + cubePosZ) };

					model = glm::translate(model, glm::vec3(i + cubePosX, k + noiseX, j + cubePosZ));
					// 4. now add to list of matrices
					//model = glm::rotate(model, (float)glfwGetTime() * speedRotation * speedMultiplier * glm::sin(glm::radians(50.0f)), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::translate(model, glm::vec3(cubePosX, cubePosY, cubePosZ));;

					modelMatrices[instanceID] = model;
					myShader.setMat4("model[" + std::to_string(instanceID) + "]", model);

					instanceID++;
				}
			}
			instanceID = 0;
			vao.Bind();
			glDrawArraysInstanced(GL_TRIANGLES, 0, 36, amount * amount);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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


#pragma region frambebuffersTest

		if (!bloom)
		{	
			// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST | GL_DEPTH_BUFFER_BIT); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		fbShader.use();
		fbShader.setBool("HDR", HDR);

		glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
		renderQuad();

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

				renderQuad();

				horizontal = !horizontal;
				if (first_iteration)
					first_iteration = false;
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
			renderQuad();

			std::cout << "bloom: " << (bloom ? "on" : "off") << "| exposure: " << exposure << std::endl;
			//clear all relevant buffers
		}



#pragma endregion


		glEnd();


#pragma region imgui

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


		if (ImGui::TreeNode("cube amount"))
		{

			const float spacing = 4;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

			ImGui::VSliderInt("##int", ImVec2(18, 160), &amount, 1, 10);
			ImGui::SameLine();

			ImGui::PopStyleVar();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("light Movement"))
		{
			const float spacing = 4;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

			static float _valueX = 0;
			ImGui::VSliderFloat("##int", ImVec2(18, 160), &_valueX, -10, 10);
			ImGui::SameLine();
			lightX = _valueX;

			static float _valueY = 0;
			ImGui::VSliderFloat("##int1", ImVec2(18, 160), &_valueY, -10, 10);
			ImGui::SameLine();
			lightY = _valueY;
			static float _valueZ = -3.0f;
			ImGui::VSliderFloat("##int2", ImVec2(18, 160), &_valueZ, -10, 10);
			ImGui::SameLine();
			lightZ = _valueZ;


			ImGui::PushID("set1");



			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Light Settings"))
		{
			// Combo Boxes are also called "Dropdown" in other systems
			// Expose flags as checkbox for the demo
			static ImGuiComboFlags flags, flags2, flags3, flags4, flags5 = 0;


			ImGui::CheckboxFlags("Directional light", &flags, ImGuiComboFlags_PopupAlignLeft);
			ImGui::CheckboxFlags("Spot light", &flags2, ImGuiComboFlags_PopupAlignLeft);
			ImGui::CheckboxFlags("Point light", &flags3, ImGuiComboFlags_PopupAlignLeft);
			ImGui::CheckboxFlags("HRD", &flags4, ImGuiComboFlags_PopupAlignLeft);
			ImGui::CheckboxFlags("BLOOM", &flags5, ImGuiComboFlags_PopupAlignLeft);




			// Using the generic BeginCombo() API, you have full control over how to display the combo contents.
			// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
			// stored in the object itself, etc.)
			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
			static int item_current_idx = 0; // Here we store our selection data as an index.
			const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
			if (ImGui::BeginCombo("combo 1", combo_preview_value, flags))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
						item_current_idx = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			// Simplified one-liner Combo() API, using values packed in a single constant string
			// This is a convenience for when the selection set is small and known at compile-time.
			static int item_current_2 = 0;
			ImGui::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

			// Simplified one-liner Combo() using an array of const char*
			// This is not very useful (may obsolete): prefer using BeginCombo()/EndCombo() for full control.
			static int item_current_3 = -1; // If the selection isn't within 0..count, Combo won't display a preview
			ImGui::Combo("combo 3 (array)", &item_current_3, items, IM_ARRAYSIZE(items));

			// Simplified one-liner Combo() using an accessor function
			struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };
			static int item_current_4 = 0;
			ImGui::Combo("combo 4 (function)", &item_current_4, &Funcs::ItemGetter, items, IM_ARRAYSIZE(items));

			dirLightOn = flags;
			spotLightOn = flags2;
			pointLightOn = flags3;
			HDR = flags4;
			bloom = flags5;


			ImGui::TreePop();
		}
		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#pragma endregion

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	//de - allocate all resources once they've outlived their purpose
	vao.~VAO();
	skyboxVAO.~VAO();
	lightVAO.~VAO();

	vbo.~VBO();
	skyboxVBO.~VBO();
	lightVBO.~VBO();
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

unsigned int LoadTexture(char const * path) {

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

unsigned int LoadCubemap(std::vector <std::string> faces) {
	unsigned int textureCubeMap;
	glGenTextures(1, &textureCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubeMap);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		//std::string file = "C:/Users/carlo/Documents/ImagesProgramming/skybox/" + textFaces[i];
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
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
void HandleLights(bool& dirLight, bool& pointLights, ShaderClass& myShader, glm:: vec3& lightPos, glm::vec3& ambientColor, glm::vec3& diffuseColor) {

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

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		unsigned int quadVAO, quadVBO;
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	
		
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}



