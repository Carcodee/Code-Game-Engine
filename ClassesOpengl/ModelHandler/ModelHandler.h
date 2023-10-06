#pragma once
#include<iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include "../Model/Model.h"
#include "../Mateial/Material.h"
#include "../Shaders/ShaderClass.h"


struct ModelItem
{
	Model newModel;
	int modelID;
	std::string name;
	glm::vec3 position;
	glm::vec3 scale;
	float rotationX, rotationY, rotationZ;
	std::string materialName;
	std::shared_ptr <Material> material;

	////generate a default constructor for this struct with parameters
	//ModelItem(int modelID, std::string name, glm::vec3 position, glm::vec3 scale, float rotationX, float rotationY, float rotationZ, std::string materialName, Material& mat) {
	//	this->modelID = modelID;
	//	this->name = name;
	//	this->position = position;
	//	this->scale = scale;
	//	this->rotationX = rotationX;
	//	this->rotationY = rotationY;
	//	this->rotationZ = rotationZ;
	//	this->materialName = materialName;
	//	this->material = mat;
	//};

	//ModelItem(const ModelItem& other) {
	//	this->modelID = other.modelID;
	//	this->name = other.name;
	//	this->position = other.position;
	//	this->scale = other.scale;
	//	this->rotationX = other.rotationX;
	//	this->rotationY = other.rotationY;
	//	this->rotationZ = other.rotationZ;
	//	this->materialName = other.materialName;
	//	this->material = other.material;
	//}

	//ModelItem& operator=(const ModelItem& other) {
	//	this->modelID = other.modelID;
	//	this->name = other.name;
	//	this->position = other.position;
	//	this->scale = other.scale;
	//	this->rotationX = other.rotationX;
	//	this->rotationY = other.rotationY;
	//	this->rotationZ = other.rotationZ;
	//	this->materialName = other.materialName;
	//	this->material = other.material;
	//	return *this;
	//}

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
		std::vector <ModelItem> models;




private:

glm::mat4 view;
glm::mat4 projection;

};

