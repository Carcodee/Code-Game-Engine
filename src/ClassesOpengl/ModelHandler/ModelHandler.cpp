#include "ModelHandler.h"
#include "../CodeObject/CodeObject.h"

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

void ModelHandler::DrawModel(ShaderClass& shader, int objectID,glm::mat4 projection, glm::mat4 view)
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
		this->currentModel = codeObjects[objectID]->transform->GetLocalModelMatrix();
		shader.setMat4("model", currentModel);
		shader.setMat4("view", this->view);
		shader.setMat4("projection", this->projection);


		codeObjects[objectID]->GetComponent<Model>()->Draw(shader);

		

}

void ModelHandler::ExtractModelMaterial(int modelID,const char* path)
{
	codeObjects[modelID]->GetComponent<Model>()->ExtractMaterials(path);
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
	return this->codeObjects[count]->transform->GetLocalModelMatrix();
}



void ModelHandler::SetModelMatrix(float* matrix, int count)
{
	this->codeObjects[count]->transform->SetLocalModelMatrix(glm::make_mat4(matrix));
}


void ModelHandler::NewCodeObject(ShaderClass* shader) {
	CodeObject* codeObject = new CodeObject(shader, this);
}

void ModelHandler::CreateCodeObject(CodeObject* codeObject)
{
	this->codeObjects.push_back(codeObject);

	codeObject->id= this->codeObjects.size()-1;
}


void ModelHandler::UpdateCodeObjects()
{
	for (int i = 0; i < this->codeObjects.size(); i++)
	{
		this->codeObjects[i]->UpdateCodeEngine();
	}
}



