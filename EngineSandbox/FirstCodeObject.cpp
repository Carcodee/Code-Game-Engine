#include "FirstCodeObject.h"




FirstCodeObject::~FirstCodeObject()
{
}

void FirstCodeObject::StartCodeEngine()
{
	AddComponent<Model>();

	model= GetComponent<Model>();
	std::shared_ptr<Material> material = std::make_shared<Material>();
	model->StartModel("Models/Cube/Cube.obj",true, material);
	//is being callled on the parent class so the is being added also in the hirearchy of the parent class level
	FirstCodeObject* newObject=new FirstCodeObject(shader,modelHandlerController);
	this->parents.push_back(newObject);


}

void FirstCodeObject::UpdateCodeEngine()
{

	model->Draw(*shader);
}
