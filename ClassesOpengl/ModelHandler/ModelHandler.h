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
	Material material;

	//generate a default constructor for this struct with parameters
	ModelItem(Model newModel,int modelID, std::string name, glm::vec3 position, glm::vec3 scale, float rotationX, float rotationY, float rotationZ, std::string materialName) {
		this->newModel = newModel;
		this->modelID = modelID;
		this->name = name;
		this->position = position;
		this->scale = scale;
		this->rotationX = rotationX;
		this->rotationY = rotationY;
		this->rotationZ = rotationZ;
		this->materialName = materialName;
		material = newModel.material;
	};


};

class ModelHandler
{
	public:
		ModelHandler();

		void AddModel(ModelItem model);
		void startLoadModel(std::string const& path, bool isPBR, int count);
		void SetModelPosition(int modelID, glm::vec3 position);
		void SetModelRotationX(int modelID, float rotation);
		void SetModelRotationY(int modelID, float rotation);
		void SetModelRotationZ(int modelID, float rotation);

		void SetModelScale(int modelID, glm::vec3 scale);
		void DrawModel(ShaderClass& shader, int modelID, glm::mat4 projection, glm::mat4 view);
		std::vector <ModelItem> models;


private:

glm::mat4 view;
glm::mat4 projection;

};

