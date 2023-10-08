#include "ModelHandler.h"

ModelHandler::ModelHandler()
{

	this->view = glm::mat4(1.0f);
	this->projection = glm::mat4(1.0f);
}

void ModelHandler::AddModel(ModelItem& modelItem)
{
	models.push_back(modelItem);
}

void ModelHandler::startLoadModel(std::string const& path, bool isPBR, std::shared_ptr<Material> material, int count)
{

	models[count].newModel.StartModel(path, isPBR, material);
}

void ModelHandler::SetModelPosition(int modelID, glm::vec3 position)
{
	models[modelID].position = position;
}

void ModelHandler::SetModelRotationX(int modelID, float rotation)
{

	models[modelID].rotationX = rotation;
}

void ModelHandler::SetModelRotationY(int modelID, float rotation)
{

	models[modelID].rotationY = rotation;
}
void ModelHandler::SetModelRotationZ(int modelID, float rotation)
{

	models[modelID].rotationZ = rotation;
}

void ModelHandler::SetModelScale(int modelID, glm::vec3 scale)
{
	models[modelID].scale = scale;
}

void ModelHandler::DrawModel(ShaderClass& shader, int modelID,glm::mat4 projection, glm::mat4 view)
{

		this->projection = projection;
		this->view = view;
		shader.use();
		//models[modelID].modelMatrix = glm::mat4(1.0f);
		//models[modelID].modelMatrix = glm::translate(models[modelID].modelMatrix, glm::vec3(models[modelID].position));
		//models[modelID].modelMatrix = glm::rotate(models[modelID].modelMatrix, glm::radians(models[modelID].rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		//models[modelID].modelMatrix = glm::rotate(models[modelID].modelMatrix, glm::radians(models[modelID].rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		//models[modelID].modelMatrix = glm::rotate(models[modelID].modelMatrix, glm::radians(models[modelID].rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
		//models[modelID].modelMatrix = glm::scale(models[modelID].modelMatrix, glm::vec3(models[modelID].scale));

		this->currentModel = models[modelID].modelMatrix ;
		shader.setMat4("model", currentModel);
		shader.setMat4("view", this->view);
		shader.setMat4("projection", this->projection);


		models[modelID].newModel.Draw(shader);

		

}

void ModelHandler::ExtractModelMaterial(int modelID,const char* path)
{
	models[modelID].newModel.ExtractMaterials(path);
}

glm::mat4 ModelHandler::GetViewMatrix()
{
	return this->view;
}

glm::mat4 ModelHandler::GetProjectionMatrix()
{
	return this->projection;
}

glm::mat4 ModelHandler::GetCurrentModelMatrix(int count)
{
	return this->models[count].modelMatrix;
}



void ModelHandler::SetModelMatrix(float* matrix, int count)
{
	this->models[count].modelMatrix = glm::make_mat4(matrix);
}

void ModelHandler::SetModelPicked(int modelID)
{
	this->modelPicked = modelID;
}

int ModelHandler::GetModelPicked()
{
	return this->modelPicked;
}





