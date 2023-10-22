#include "CodeObject.h"
#include "../src/ClassesOpengl/ModelHandler/ModelHandler.h"

CodeObject::CodeObject(ShaderClass* shader, ModelHandler* model)
{
	this->shader =shader;
	this->modelHandlerController = model;
	modelHandlerController->CreateCodeObject(this);
	this->name = "Code Object " + id;

	AddComponent<Transform>();
	shader = GetComponent<ShaderClass>();

	StartCodeEngine();
}

CodeObject::~CodeObject()
{

}

void CodeObject::StartCodeEngine()
{

}

void CodeObject::UpdateCodeEngine()
{
}


void CodeObject::SetShaderProperties()
{
}



