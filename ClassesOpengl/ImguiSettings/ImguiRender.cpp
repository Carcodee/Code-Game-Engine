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

void ImguiRender::CreateViewPort(unsigned int textureID)
{

	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove);
	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	//ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH, SCR_HEIGHT));
	//maintain the window the same size of the glfw window
	ImGui::GetWindowDrawList()->AddImage(
		(void*)textureID,
		ImVec2(ImGui::GetCursorScreenPos()),
		viewportSize,
		ImVec2(0, 1),
		ImVec2(1, 0));

}


ImguiRender::~ImguiRender()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
