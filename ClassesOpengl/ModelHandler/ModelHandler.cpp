#include "ModelHandler.h"

ModelHandler::ModelHandler()
{

	this->view = glm::mat4(1.0f);
	this->projection = glm::mat4(1.0f);
}

void ModelHandler::AddModel(ModelItem modelItem)
{
	models.push_back(modelItem);
}

void ModelHandler::startLoadModel(std::string const& path, bool isPBR, int count)
{
	models[count].newModel.StartModel(path, isPBR);
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
		glm::mat4 modelM = glm::mat4(1.0f);
		modelM = glm::translate(modelM, glm::vec3(models[modelID].position)); 
		modelM = glm::rotate(modelM, glm::radians(models[modelID].rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
		modelM = glm::rotate(modelM, glm::radians(models[modelID].rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
		modelM = glm::rotate(modelM, glm::radians(models[modelID].rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
		modelM = glm::scale(modelM, glm::vec3(models[modelID].scale));	


		shader.setMat4("model", modelM);
		shader.setMat4("view", this->view);
		shader.setMat4("projection", this->projection);


		models[modelID].newModel.Draw(shader);

	

}
