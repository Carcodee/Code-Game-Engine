#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include "imgui/ImGuizmo.h"
#include "../ClassesOpengl/ModelHandler/ModelHandler.h"
class ImguiRender
{
	public:
	ImguiRender(GLFWwindow* window);
	~ImguiRender();
	void NewFrame();
	void Render();
	template <typename Lambda>
	void CreateNode(Lambda myLambda) {
		myLambda();
	};

	void CreateViewPort(unsigned int textureID,ModelHandler& modelHandler);
	private:
};

