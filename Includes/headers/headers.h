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

#include "../ClassesOpengl/VAO.h"
#include "../ClassesOpengl/VBO.h"
#include "../ClassesOpengl/EBO.h"
#include "../Shaders/ShaderClass.h"
#include "../Mesh/Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Noise/PerlinNoise.h"
#include "OBJLoader/OBJ_Loader.h"
#include "../Model/Model.h"
#include "../Mesh/Mesh.h"
#include "../Camera/CameraHandler.h"
#include "../ClassesOpengl/Framebuffer.h"
#include "stb-master/stb_image.h"
#include "../ClassesOpengl/Mateial/Material.h"
#include "../ClassesOpengl/ModelHandler/ModelHandler.h"

#define STB_IMAGE_IMPLEMENTATION
