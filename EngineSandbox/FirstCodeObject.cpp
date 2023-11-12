#include "FirstCodeObject.h"




FirstCodeObject::~FirstCodeObject()
{
}

void FirstCodeObject::StartCodeEngine()
{
	//is being callled on the parent class so the is being added also in the hirearchy of the parent class level


	model= AddComponent<Model>();
	std::shared_ptr<Material> material = std::make_shared<Material>();
	model->StartModel("Assets/Models/box/Cube.obj",true, material);
	FirstCodeObject* newObject=new FirstCodeObject(shader,modelHandlerController);
	this->AddParent(newObject);


}

void FirstCodeObject::UpdateCodeEngine()
{

}
