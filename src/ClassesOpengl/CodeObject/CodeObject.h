#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../src/ClassesOpengl/Mateial/Material.h"
#include "../src/Model/Model.h"
#include "../src/ClassesOpengl/TransformClass/Transform.h"
#include "ComponentBase/ComponentBase.h"
#include "../Shaders/Classes/ShaderClass.h"
//#include "../src/ClassesOpengl/ModelHandler/ModelHandler.h"
class ModelHandler;

class CodeObject
{
public:
	CodeObject(ShaderClass* shader, ModelHandler* model);
	~CodeObject();

	virtual void StartCodeEngine();
	virtual void UpdateCodeEngine();
	//material
	template<typename T>
	void AddComponent();

	template<typename T>
	T* GetComponent();
	
	
	std::string name;
	int id;
	std::vector<ComponentBase*> components;
	std::vector<CodeObject*> parents;
	ShaderClass *shader;
	ModelHandler* modelHandlerController;
private: 
	void SetShaderProperties();
};

template<typename T>
T* CodeObject::GetComponent()
{
    for (int i = 0; i < components.size(); i++)
	{
		T* derivedComponent = dynamic_cast<T*>(components[i]);

		if (derivedComponent != nullptr)
		{
			std::cout << "Component found" << std::endl;
			return derivedComponent;
		}
	}
	std::cout << "Component Not found" << std::endl;

	return nullptr;
}
template<typename T>
void CodeObject::AddComponent()
{
	T* component = new T();
	components.push_back(component);
}
