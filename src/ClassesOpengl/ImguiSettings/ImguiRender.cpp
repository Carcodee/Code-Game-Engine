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
	selected = 0;
	currentDirectory= "Assets";
	relativeAssetsPath = "Assets";
	dragDropPath = "";
	contentBrowserFilePath = util::LoadTexture("Resources/ContentBrowser/FileIcon.png");
	contentBrowserIconPath = util::LoadTexture("Resources/ContentBrowser/FolderIcon.png");

}

void ImguiRender::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImguiRender::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiRender::SetModelHandler(ModelHandler& modelHandler)
{
	this->myModelHandler = modelHandler;
}

void ImguiRender::CreateViewPort(unsigned textID, ModelHandler& modelHandler)
{
	//lock viewport to the size of the window

	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
	ImGui::Begin("Viewport", 0 ,ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);

	ImVec2 viewportSize = ImGui::GetContentRegionAvail();
	if (myViewportSize.x != viewportSize.x || myViewportSize.y != viewportSize.y)
	{
		//resize
		glViewport(0, 0, viewportSize.x, viewportSize.y);
		myFrameBuffer.Resize((int)viewportSize.x, (int)viewportSize.y);
		myViewportSize = { viewportSize.x,viewportSize.y };
		std::cout << myFrameBuffer.m_Width << std::endl;
		std::cout << myFrameBuffer.m_Height << std::endl;

	}

	ImGui::Image((void*)myFrameBuffer.m_Texture, {myViewportSize.x, myViewportSize.y}, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
			const wchar_t* path = (const wchar_t*)payload->Data;
			dragDropPath=(relativeAssetsPath/std::filesystem::path(path)).string();
			std::cout<<dragDropPath<<std::endl;
			ImGui::EndDragDropTarget();
			dragDropSucced = true;

		}
	}
	CreateGuizmos(modelHandler);



	ImGui::End();

	//ImGui::PopStyleVar();

}

void ImguiRender::CreateGuizmos(ModelHandler& modelHandler)
{
	if (modelHandler.codeObjects.size() > 0)
	{
		ImGuiIO& io = ImGui::GetIO();
		float viewManipulateRight = io.DisplaySize.x;
		float viewManipulateTop = 0;
		static ImGuiWindowFlags gizmoWindowFlags = 0;

		ImGuizmo::SetDrawlist();
		float windowWidth = myViewportSize.x;
		float windowHeight = myViewportSize.y;
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
		viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
		viewManipulateTop = ImGui::GetWindowPos().y;
		float* model = (float*)glm::value_ptr(modelHandler.GetCurrentModelMatrix(selected));

		ImGuizmo::Manipulate(glm::value_ptr(modelHandler.GetViewMatrix()), glm::value_ptr(modelHandler.GetProjectionMatrix()),
			mCurrentGizmoOperation, ImGuizmo::MODE::LOCAL, model,
			NULL, NULL, NULL, NULL);
		modelHandler.SetModelMatrix(model, selected);
		ImGuizmo::ViewManipulate((float*)glm::value_ptr(modelHandler.GetViewMatrix()), 8.0f, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

	}
}
void ImguiRender::CreateContentBrowser()
{
	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Content Browser"))
	{
		ImGui::End();
		return;
	}

	std::filesystem::path pathToShow = currentDirectory;
	if (currentDirectory != std::filesystem::path(relativeAssetsPath))
	{
		if (ImGui::Button("<-"))
		{
			currentDirectory = pathToShow.parent_path();
		}
	}
	static  float padding = 16.0f;
	static  float thumbNailSize= 128.0f;
	float cellSize = thumbNailSize + padding;
	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
	{
		columnCount = 1;
	}


	ImGui::Columns(columnCount,0,false);
	for (const auto& entry : std::filesystem::directory_iterator(pathToShow))
	{
		std::string pathString = entry.path().string();	
		auto relativePath = std::filesystem::relative(pathString, relativeAssetsPath);
		std::string filename= relativePath.filename().string();
		contentBrowserTextureID = (entry.is_directory())? contentBrowserIconPath : contentBrowserFilePath;
		(ImGui::ImageButton((ImTextureID)contentBrowserTextureID, { thumbNailSize,thumbNailSize },{1,0},{0,1}));

		if (ImGui::BeginDragDropSource())
		{

			const wchar_t* itemPath = relativePath.c_str();

			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1 )* sizeof(wchar_t),ImGuiCond_Once);
			ImGui::EndDragDropSource();
		}
		if (ImGui::IsItemHovered()&&ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (entry.is_directory())
			{
				currentDirectory /= entry.path().filename();
			}
		}
		ImGui::Text(filename.c_str());

		ImGui::NextColumn();

	}

	ImGui::Columns(1);
	ImGui::SliderFloat("ThumbnailSize",&thumbNailSize, 16,512);
	ImGui::SliderFloat("Padding", &padding, 0, 32);

	ImGui::End();
}

void ImguiRender::CreateHirearchy(std::vector<CodeObject*> objects)
{
	ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Example: Property editor"))	{

		ImGui::End();
		return;
	}


	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
	if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
	{
		// Iterate placeholder objects (all the same data)
		for (int obj_i = 0; obj_i < objects.size(); obj_i++)
		{
			if (objects[obj_i]->showed)continue;
			ShowPlaceholderObject("Object", objects[obj_i]);
			//ImGui::Separator();
		}
		ImGui::EndTable();
	}
	ImGui::PopStyleVar();
	ImGui::End();
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

void ImguiRender::SetFrameBuffer(Framebuffer& frameBuffer)
{
	this->myFrameBuffer = frameBuffer;
}

void ImguiRender::OnDragDropCallBack(DragDropFileType filetype)
{
	if (dragDropSucced)
	{
		switch (filetype)
		{
		case fbx:
			myModelHandler.DragDropCodeObject(dragDropPath.c_str());
			break;
		case png:
			break;
		case jpg:
			break;
		case folder:
			break;
		default:
			break;
		}
		dragDropSucced = false;
	}
	
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
		for (int i = 0; i < object->parents.size(); i++)
		{
			object->parents[i]->showed = true;
		}
		ImGui::TableSetColumnIndex(1);
		ImGui::Text("my sailor is rich");

		if (node_open)
		{
			static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
			selected = object->id;

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
