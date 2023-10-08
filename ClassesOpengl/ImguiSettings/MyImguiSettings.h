#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <iostream>
#include "../ClassesOpengl/ModelHandler/ModelHandler.h"
#include "../Model/Model.h"
auto MoveCubes = [](float& speed, float& rotationSpeed) {



	if (ImGui::TreeNode("Cube Speed"))
	{
		const float spacing = 4;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

		static int int_value = 1;
		ImGui::VSliderInt("##int", ImVec2(18, 160), &int_value, 1, 10);
		ImGui::SameLine();
		speed = int_value;
		static float values[1] = { 0.0f };
		rotationSpeed = values[0];
		ImGui::PushID("set1");
		for (int i = 0; i < 1; i++)
		{
			if (i > 0) ImGui::SameLine();
			ImGui::PushID(i);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV(i / 7.0f, 0.5f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV(i / 7.0f, 0.7f, 0.5f));
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV(i / 7.0f, 0.9f, 0.9f));
			ImGui::VSliderFloat("##v", ImVec2(18, 160), &values[i], -1.0f, 1.0f, "");
			if (ImGui::IsItemActive() || ImGui::IsItemHovered())
				ImGui::SetTooltip("%.3f", values[i]);
			ImGui::PopStyleColor(4);
			ImGui::PopID();
			ImGui::Spacing();
		}

		ImGui::PopID();

		ImGui::SameLine();
		ImGui::PushID("set3");
		for (int i = 0; i < 1; i++)
		{
			if (i > 0) ImGui::SameLine();
			ImGui::PushID(i);
			ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
			ImGui::VSliderFloat("##v", ImVec2(40, 160), &values[i], -1.0f, 1.0f, "%.2f\nsec");
			ImGui::PopStyleVar();
			ImGui::PopID();
		}
		ImGui::PopID();
		ImGui::PopStyleVar();
		ImGui::TreePop();
		ImGui::Spacing();

	}
};
auto cubes_Movement=[](float& cubePosX, float& cubePosY, float& cubePosZ,float& noiseMult ) {
		if (ImGui::TreeNode("Cube Movement"))
		{
			const float spacing = 4;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

			static float _valueX = 0;
			ImGui::VSliderFloat("##int", ImVec2(18, 160), &_valueX, -10, 10);
			ImGui::SameLine();
			cubePosX = _valueX;

			static float _valueY = 0;
			ImGui::VSliderFloat("##int1", ImVec2(18, 160), &_valueY, -10, 10);
			ImGui::SameLine();
			cubePosY = _valueY;
			static float _valueZ = -3.0f;
			ImGui::VSliderFloat("##int2", ImVec2(18, 160), &_valueZ, -10, 10);
			ImGui::SameLine();
			cubePosZ = _valueZ;
			static float myNoiseMult = 0;
			ImGui::VSliderFloat("##int3", ImVec2(18, 160), &myNoiseMult, -10, 10);
			ImGui::SameLine();
			noiseMult = myNoiseMult;


			ImGui::PushID("set1");

			ImGui::PopID();
			ImGui::PopStyleVar();
			ImGui::TreePop();
			ImGui::Spacing();

		}

		

	};
auto cube_Amount = [](int& amount, float& roughness, float& metalness, float& ao, float& albedo) {
	if (ImGui::TreeNode("cube amount"))
	{

		const float spacing = 4;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

		ImGui::VSliderInt("##int", ImVec2(18, 160), &amount, 1, 10);

		static float f1 = 1.0f;
		ImGui::SliderFloat("Rougness", &f1, 0.0f, 5.0f, "%.4f");
		roughness = f1;
		static float f2 = 1.0f;
		ImGui::SliderFloat("Metalness", &f2, 0.0f, 5.0f, "%.4f");
		metalness = f2;
		static float f3 = 1.0f;
		ImGui::SliderFloat("Ao intensity", &f3, 0.0f, 5.0f, "%.4f");
		ao = f3;
		static float f4 = 1.0f;
		ImGui::SliderFloat("albedo Intensity", &f4, 0.0f, 5.0f, "%.4f");
		albedo = f4;

		ImGui::PopStyleVar();
		ImGui::TreePop();
		ImGui::Spacing();



	}

	};
auto light_Movement = [](float& lightX,float& lightY,float& lightZ) {

	if (ImGui::TreeNode("light Movement"))
	{
		const float spacing = 4;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

		static float _valueX = 0;
		ImGui::VSliderFloat("##int", ImVec2(18, 160), &_valueX, -10, 10);
		ImGui::SameLine();
		lightX = _valueX;

		static float _valueY = 0;
		ImGui::VSliderFloat("##int1", ImVec2(18, 160), &_valueY, -10, 10);
		ImGui::SameLine();
		lightY = _valueY;
		static float _valueZ = -3.0f;
		ImGui::VSliderFloat("##int2", ImVec2(18, 160), &_valueZ, -10, 10);
		ImGui::SameLine();
		lightZ = _valueZ;


		ImGui::PushID("set1");



		ImGui::PopID();
		ImGui::PopStyleVar();
		ImGui::TreePop();
		ImGui::Spacing();

	}
	};
auto height_Mapping = [](float& heightScaleFactor) {
	if (ImGui::TreeNode("Height Mapping"))
	{
		const float spacing = 4;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

		static float myValue = 0.1f;
		ImGui::VSliderFloat("##int", ImVec2(18, 160), &myValue, 0.0, 1.0f);
		ImGui::SameLine();
		heightScaleFactor = myValue;

		ImGui::PushID("set2");
		ImGui::PopID();
		ImGui::PopStyleVar();
		ImGui::TreePop();
		ImGui::Spacing();

	}

	};
auto light_Settings = [](bool& dirLightOn, bool& spotLightOn, bool& pointLightOn, bool& HDR, bool& bloom, bool& shadows) {
	if (ImGui::TreeNode("Light Settings"))
	{
		// Combo Boxes are also called "Dropdown" in other systems
		// Expose flags as checkbox for the demo
		static ImGuiComboFlags flags, flags2, flags3, flags4, flags5, flags6 = 1;


		ImGui::CheckboxFlags("Directional light", &flags, ImGuiComboFlags_PopupAlignLeft);
		ImGui::CheckboxFlags("Spot light", &flags2, ImGuiComboFlags_PopupAlignLeft);
		ImGui::CheckboxFlags("Point light", &flags3, ImGuiComboFlags_PopupAlignLeft);
		ImGui::CheckboxFlags("HRD", &flags4, ImGuiComboFlags_PopupAlignLeft);
		ImGui::CheckboxFlags("BLOOM", &flags5, ImGuiComboFlags_PopupAlignLeft);
		ImGui::CheckboxFlags("Shadows", &flags6, ImGuiComboFlags_PopupAlignLeft);





		// Using the generic BeginCombo() API, you have full control over how to display the combo contents.
		// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
		// stored in the object itself, etc.)
		const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
		static int item_current_idx = 0; // Here we store our selection data as an index.
		const char* combo_preview_value = items[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
		if (ImGui::BeginCombo("combo 1", combo_preview_value, flags))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		// Simplified one-liner Combo() API, using values packed in a single constant string
		// This is a convenience for when the selection set is small and known at compile-time.
		static int item_current_2 = 0;
		ImGui::Combo("combo 2 (one-liner)", &item_current_2, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");

		// Simplified one-liner Combo() using an array of const char*
		// This is not very useful (may obsolete): prefer using BeginCombo()/EndCombo() for full control.
		static int item_current_3 = -1; // If the selection isn't within 0..count, Combo won't display a preview
		ImGui::Combo("combo 3 (array)", &item_current_3, items, IM_ARRAYSIZE(items));

		// Simplified one-liner Combo() using an accessor function
		struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };
		static int item_current_4 = 0;
		ImGui::Combo("combo 4 (function)", &item_current_4, &Funcs::ItemGetter, items, IM_ARRAYSIZE(items));

		dirLightOn = flags;
		spotLightOn = flags2;
		pointLightOn = flags3;
		HDR = flags4;
		bloom = flags5;
		shadows = flags6;


		ImGui::TreePop();
		ImGui::Spacing();

	}

	};

auto model_configs = [](ModelHandler& myModelHandler) {

	if (ImGui::TreeNode("MODELS DATA"))
	{
		const float spacing1 = 4;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing1, spacing1));

		for (size_t i = 0; i < myModelHandler.models.size(); i++)
		{
			if (myModelHandler.models[i].newModel.isLoaded)
			{
				std::string name = myModelHandler.models[i].name +" " + std::to_string(myModelHandler.models[i].modelID);
				if (ImGui::TreeNode(name.c_str()))
				{
			
					if (ImGui::TreeNode("MaterialConfigs: "))
					{
				
						static float col1[3] = { 1.0f, 1.0f, 1.0f };

						ImGui::ColorPicker3("Material Color", col1);
						myModelHandler.models[i].material->SetMaterialColor(glm::vec3(col1[0], col1[1], col1[2]));

						
						static float f4 = 1.0f;
						ImGui::SliderFloat("albedo Intensity", &f4, 0.0f, 5.0f, "%.4f");

						static float f2 = 1.0f;
						ImGui::SliderFloat("Metalness", &f2, 0.0f, 5.0f, "%.4f");

						static float f1 = 1.0f;
						ImGui::SliderFloat("Rougness", &f1, 0.0f, 5.0f, "%.4f");

						static float f3 = 1.0f;
						ImGui::SliderFloat("Ao intensity", &f3, 0.0f, 5.0f, "%.4f");
						
						
						myModelHandler.models[i].material->SetMaterialProperties(f4,f1, f2, f3);



						ImGui::TreePop();
						ImGui::Spacing();


					}
					if (ImGui::TreeNode("TRANSFORM"))
					{
						
							ImGui::Text("Position");
							float matrixTranslation[3], matrixRotation[3], matrixScale[3];
							ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(myModelHandler.models[i].modelMatrix), matrixTranslation, matrixRotation, matrixScale);
							ImGui::InputFloat3("Translate", matrixTranslation);
							ImGui::InputFloat3("Rotate", matrixRotation);
							ImGui::InputFloat3("Scale", matrixScale);
							ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, glm::value_ptr(myModelHandler.models[i].modelMatrix));
							

						ImGui::TreePop();
						ImGui::Spacing();
					}
					struct TextFilters
					{
						// Modify character input by altering 'data->Eventchar' (ImGuiInputTextFlags_CallbackCharFilter callback)
						static int FilterCasingSwap(ImGuiInputTextCallbackData* data)
						{
							if (data->EventChar >= 'a' && data->EventChar <= 'z') { data->EventChar -= 'a' - 'A'; } // Lowercase becomes uppercase
							else if (data->EventChar >= 'A' && data->EventChar <= 'Z') { data->EventChar += 'a' - 'A'; } // Uppercase becomes lowercase
							return 0;
						}

						// Return 0 (pass) if the character is 'i' or 'm' or 'g' or 'u' or 'i', otherwise return 1 (filter out)
						static int FilterImGuiLetters(ImGuiInputTextCallbackData* data)
						{
							if (data->EventChar < 256 && strchr("imgui", (char)data->EventChar))
								return 0;
							return 1;
						}
					};

					static char buf1[400] = ""; ImGui::InputText("Model path here:", buf1, 400);

					if (ImGui::Button("Extract material")) {
						//"Models/BackpackModel/backpack.obj"
						//Models/pizzaCar/myPizzaMovil.obj
						//Models/pig/pig.obj
						myModelHandler.ExtractModelMaterial(i, buf1);
					}
					ImGui::TreePop();
					ImGui::Spacing();

				}

			
			}
		}
		ImGui::PopStyleVar();
		ImGui::TreePop();
		ImGui::Spacing();
	}

};
auto model_LoaderTest = [](ModelHandler& models, bool& flipUVS, int& modelCounter, std::string& path) {

	if (ImGui::TreeNode("Model Loader"))
	{
		struct TextFilters
		{
			// Modify character input by altering 'data->Eventchar' (ImGuiInputTextFlags_CallbackCharFilter callback)
			static int FilterCasingSwap(ImGuiInputTextCallbackData* data)
			{
				if (data->EventChar >= 'a' && data->EventChar <= 'z') { data->EventChar -= 'a' - 'A'; } // Lowercase becomes uppercase
				else if (data->EventChar >= 'A' && data->EventChar <= 'Z') { data->EventChar += 'a' - 'A'; } // Uppercase becomes lowercase
				return 0;
			}

			// Return 0 (pass) if the character is 'i' or 'm' or 'g' or 'u' or 'i', otherwise return 1 (filter out)
			static int FilterImGuiLetters(ImGuiInputTextCallbackData* data)
			{
				if (data->EventChar < 256 && strchr("imgui", (char)data->EventChar))
					return 0;
				return 1;
			}
		};

		static char buf1[400] = ""; ImGui::InputText("Model path here:", buf1, 400);
		// Save/Revert button
		static ImGuiComboFlags flipUvs = 1;
		static ImGuiComboFlags PBRon = 1;

		ImGui::CheckboxFlags("Flip UVS", &flipUvs, ImGuiComboFlags_PopupAlignLeft);
		ImGui::CheckboxFlags("PBR", &PBRon, ImGuiComboFlags_PopupAlignLeft);

		if (ImGui::Button("Load Model")) {
			//"Models/BackpackModel/backpack.obj"
			//Models/pizzaCar/myPizzaMovil.obj
			//Models/pig/pig.obj
			std::string strPath = path;
			flipUVS = flipUvs;
			std::shared_ptr<Material> material = std::make_shared<Material>();
			ModelItem model = { Model(),modelCounter, glm::mat4(1.0f),"Cube ", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.0f, "Base", material};
			models.AddModel(model);
			models.startLoadModel(strPath, PBRon, material,modelCounter);
			modelCounter++;
		}
		if (ImGui::Button("Load Cube")) {
			//"Models/BackpackModel/backpack.obj"
			//Models/pizzaCar/myPizzaMovil.obj
			//Models/pig/pig.obj
			std::string strPath = "Models/Cube/Cube.obj";
			flipUVS = flipUvs;
			std::shared_ptr<Material> material = std::make_shared<Material>();
			ModelItem model = { Model(),modelCounter, glm::mat4(1.0f),"Cube ", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f, 0.0f, "Base", material};
			models.AddModel(model);
			//the problem is that the material that i give to the item is not being used, instead i am creating one in the model class

			models.startLoadModel(strPath, PBRon, material, modelCounter);
			modelCounter++;
		}

		ImGui::SameLine();
		ImGui::TreePop();
		ImGui::Spacing();
	};
	};
