#pragma once
#include<iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "../src/Model/Model.h"
#include "../Mateial/Material.h"
#include "../Shaders/Classes/ShaderClass.h"
#include "../CodeObject/CodeObject.h"

struct ModelItem
{
	Model newModel;
	int modelID;
	glm::mat4 modelMatrix;
	std::string name;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationX, rotationY, rotationZ;
	std::string materialName;
	std::shared_ptr <Material> material;
};

class ModelHandler
{
	public:
		ModelHandler();

		void AddModel(ModelItem& model);
		void startLoadModel(std::string const& path, bool isPBR, std::shared_ptr<Material> material, int count);
		void SetModelPosition(int modelID, glm::vec3 position);
		void SetModelRotationX(int modelID, float rotation);
		void SetModelRotationY(int modelID, float rotation);
		void SetModelRotationZ(int modelID, float rotation);

		void SetModelScale(int modelID, glm::vec3 scale);
		void DrawModel(ShaderClass& shader, int modelID, glm::mat4 projection, glm::mat4 view);
		void ExtractModelMaterial(int modelID, const char* path);

		glm::mat4 GetViewMatrix();
		glm::mat4 GetProjectionMatrix();
		glm::mat4 GetCurrentModelMatrix(int count);
		void SetModelMatrix(float* matrix, int count);
		std::vector <ModelItem> models;
		void SetModelPicked(int modelID);
		int GetModelPicked();

		//codeobject
		std::vector<CodeObject> codeObjects;

private:
	int modelPicked;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 currentModel;
	int counter;
};

