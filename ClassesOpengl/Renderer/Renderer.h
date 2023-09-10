#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include "../ClassesOpengl/VAO.h"
#include "../ClassesOpengl/VBO.h"
#include "../ClassesOpengl/EBO.h"
#include "../Shaders/ShaderClass.h"
#include "../Model/Model.h"

class Renderer

{
public:
	Renderer();
	void RenderElements(ShaderClass& shader, std::vector<int> indices, VAO vao);
	void RenderArrays(ShaderClass& shader, std::vector<float> vertices, VAO vao);
    void RenderInstancedArrays(ShaderClass& shader, std::vector<float> vertices, VAO vao, unsigned int amount);
	void RenderModel(ShaderClass& shader,Model model);
	~Renderer();

private:

};

