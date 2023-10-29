#include "CodeObject.h"
#include "../src/ClassesOpengl/ModelHandler/ModelHandler.h"

CodeObject::CodeObject(ShaderClass* shader, ModelHandler* model)
{
	this->showed = false;
	this->shader =shader;
	this->modelHandlerController = model;
	this->parents.clear();
	modelHandlerController->CreateCodeObject(this);
	this->name = "Code Object " +std::to_string(id);
	std::cout << name << std::endl;

	AddComponent<Transform>();
	shader = GetComponent<ShaderClass>();
	transform = GetComponent<Transform>();
}

CodeObject::~CodeObject()
{

}

void CodeObject::Implementation()
{

}

void CodeObject::StartCodeEngine()
{

}

void CodeObject::UpdateCodeEngine()
{
}

void CodeObject::AddParent(CodeObject* parent)
{
	parents.push_back(parent);
}


void CodeObject::SetShaderProperties()
{
}



