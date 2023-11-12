#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <thread>
#include <future>
#include <memory>

#include "../src/ClassesOpengl/OpenGLHelpers/VAO.h"
#include "../src/ClassesOpengl/OpenGLHelpers/VBO.h"
#include "../src/ClassesOpengl/OpenGLHelpers/EBO.h"
#include "../Shaders/Classes/ShaderClass.h"
#include "../src/Mesh/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Noise/PerlinNoise.h"
#include "OBJLoader/OBJ_Loader.h"
#include "../src/Model/Model.h"
#include "../src/Mesh/Mesh.h"
#include "../src/Camera/CameraHandler.h"
#include "../src/ClassesOpengl/OpenGLHelpers/Framebuffer.h"
#include "../src/ClassesOpengl/Mateial/Material.h"
#include "../src/ClassesOpengl/ModelHandler/ModelHandler.h"
#include "../src/Skybox/Skybox.h"
#include "stb-master/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../src/GameEngineActions/EventSystem/EventSystem.h"
#include"../src/GameEngineActions/IUndoable.h"

#include "../EngineSandbox/FirstCodeObject.h"