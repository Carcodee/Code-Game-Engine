#include "CodeObject.h"

CodeObject::CodeObject(std::string name, int id, ShaderClass* shader)
{
	this->name = name;
	this->id = id;
	this->shader =shader;
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



