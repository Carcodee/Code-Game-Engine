#include "ImguiRender.h"
#include "../CodeObject/CodeObject.h"

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
	viewportWindowSize = ImGui::GetWindowSize();
	glViewport(0, 0, viewportWindowSize.x, viewportWindowSize.y);
	//ImGui::SetNextWindowSize(ImVec2(SCR_WIDTH, SCR_HEIGHT));
	//maintain the window the same size of the glfw window
	ImGui::GetWindowDrawList()->AddImage(
		(void*)textureID,
		ImVec2(ImGui::GetCursorScreenPos()),
		viewportWindowSize,
		ImVec2(0, 1),
		ImVec2(1, 0));
	CreateGuizmos(modelHandler);

	


}

void ImguiRender::CreateContentBrowser()
{
	ImGui::Begin("Content Browser");
	ImGui::End();
}

void ImguiRender::CreateHirearchy(std::vector<CodeObject*> objects)
{
	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Example: Property editor"))
	{
		ImGui::End();
		return;
	}


	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
	{
		// Iterate placeholder objects (all the same data)
		for (int obj_i = 0; obj_i < objects.size(); obj_i++)
		{
			ShowPlaceholderObject("Object", objects[obj_i]);
			//ImGui::Separator();
		}
		ImGui::EndTable();
	}
	ImGui::PopStyleVar();
	ImGui::End();
}

void ImguiRender::CreateGuizmos(ModelHandler& modelHandler)
{
	if (modelHandler.models.size() > 0)
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
		float* model = (float*)glm::value_ptr(modelHandler.GetCurrentModelMatrix(modelHandler.GetModelPicked()));

		ImGuizmo::Manipulate(glm::value_ptr(modelHandler.GetViewMatrix()), glm::value_ptr(modelHandler.GetProjectionMatrix()),
			mCurrentGizmoOperation, ImGuizmo::MODE::LOCAL, model,
			NULL, NULL, NULL, NULL);
		modelHandler.SetModelMatrix(model, modelHandler.GetModelPicked());
		ImGuizmo::ViewManipulate((float*)glm::value_ptr(modelHandler.GetViewMatrix()), 8.0f, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

	}
}

void ImguiRender::SetGizmoOperation(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		mCurrentGizmoOperation = ImGuizmo::SCALE;

}

void ImguiRender::ShowPlaceholderObject(const char* prefix, CodeObject* object)
{

		// Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
		ImGui::PushID(object->id);

		// Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::AlignTextToFramePadding();
		bool node_open = ImGui::TreeNode(object->name.c_str(), "%s_%u", prefix, object->id);
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("my sailor is rich");

		if (node_open)
		{
			static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
			for (int i = 0; i < object->parents.size(); i++)
			{
				ImGui::PushID(i); // Use field index as identifier.
				if (i < 2)
				{
					ShowPlaceholderObject(object->name.c_str(), object->parents[i]);
				}
				else
				{
					// Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
					ImGui::TreeNodeEx("Field", flags, "Field_%d", i);

					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(-FLT_MIN);
					if (i >= 5)
						ImGui::InputFloat("##value", &placeholder_members[i], 1.0f);
					else
						ImGui::DragFloat("##value", &placeholder_members[i], 0.01f);
					ImGui::NextColumn();
				}
				ImGui::PopID();
			}
			ImGui::TreePop();
		}
		ImGui::PopID();
}



ImguiRender::~ImguiRender()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
