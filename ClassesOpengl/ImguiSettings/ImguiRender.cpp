#include "ImguiRender.h"

ImguiRender::ImguiRender(GLFWwindow* window)
{
	//IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controlbs
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
}

void ImguiRender::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImguiRender::Render()
{
	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiRender::CreateViewPort(unsigned int textureID, ModelHandler& modelHandler)
{

	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove);
	ImVec2 viewportSize = ImGui::GetWindowSize();
	glViewport(0, 0, viewportSize.x, viewportSize.y);
	//ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH, SCR_HEIGHT));
	//maintain the window the same size of the glfw window
	ImGui::GetWindowDrawList()->AddImage(
		(void*)textureID,
		ImVec2(ImGui::GetCursorScreenPos()),
		viewportSize,
		ImVec2(0, 1),
		ImVec2(1, 0));
	for (size_t i = 0; i < modelHandler.models.size(); i++)
	{

		ImGuiIO& io = ImGui::GetIO();
		float viewManipulateRight = io.DisplaySize.x;
		float viewManipulateTop = 0;
		static ImGuiWindowFlags gizmoWindowFlags = 0;

		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
		viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
		viewManipulateTop = ImGui::GetWindowPos().y;
		float * model= (float*)glm::value_ptr(modelHandler.GetCurrentModelMatrix(i));

		ImGuizmo::Manipulate(glm::value_ptr(modelHandler.GetViewMatrix()), glm::value_ptr(modelHandler.GetProjectionMatrix()),
			ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::MODE::LOCAL, model,
			NULL, NULL, NULL, NULL);
		modelHandler.SetModelMatrix(model,i);
		ImGuizmo::ViewManipulate((float*)glm::value_ptr(modelHandler.GetViewMatrix()), 8.0f, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);
	
	}


}


ImguiRender::~ImguiRender()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
