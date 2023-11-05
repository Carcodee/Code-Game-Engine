#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <filesystem>
#include "imgui/ImGuizmo.h"
#include "../src/ClassesOpengl/ModelHandler/ModelHandler.h"
#include "../src/Functions/Utility.h"
#include "../src/ClassesOpengl/OpenGLHelpers/Framebuffer.h"
enum DragDropFileType
{
	fbx,
	png,
	jpg,
	folder
};

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
	void SetModelHandler(ModelHandler& modelHandler);
	ImVec2 viewportWindowSize;
	void CreateViewPort(unsigned textID,ModelHandler& modelHandler);
	void CreateContentBrowser();
	void CreateHirearchy(std::vector<CodeObject*> objects);
	void inline CreateGuizmos(ModelHandler& modelHandler);
	void SetGizmoOperation(GLFWwindow* window);
	void SetFrameBuffer(Framebuffer& frameBuffer);

	void OnDragDropCallBack(DragDropFileType filetype);
	int selected;
	private:
		ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGuizmo::MODE mCurrentGizmoMode = ImGuizmo::WORLD;

	    void ShowPlaceholderObject(const char* prefix,CodeObject* object);

		//browser
		std::filesystem::path currentDirectory;
		std::filesystem::path relativeAssetsPath;
		bool dragDropSucced=false;
		std::string dragDropPath;
		unsigned int contentBrowserIconPath;
		unsigned int contentBrowserFilePath;
		unsigned int contentBrowserTextureID;
        ImVec2 myViewportSize;
		Framebuffer myFrameBuffer;
		ModelHandler myModelHandler;
};

