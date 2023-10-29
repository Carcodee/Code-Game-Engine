#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include "imgui/ImGuizmo.h"
#include "../src/ClassesOpengl/ModelHandler/ModelHandler.h"

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
	ImVec2 viewportWindowSize;
	void CreateViewPort(unsigned int textureID,ModelHandler& modelHandler);
	void CreateContentBrowser();
	void CreateHirearchy(std::vector<CodeObject*> objects);
	void CreateGuizmos(ModelHandler& modelHandler);
	void SetGizmoOperation(GLFWwindow* window);
	int selected;
	private:
		ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::WORLD;

	    void ShowPlaceholderObject(const char* prefix,CodeObject* object);
        

        
};

