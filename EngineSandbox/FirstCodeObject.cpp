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



}

void FirstCodeObject::UpdateCodeEngine()
{

	model->Draw(*shader);
}
