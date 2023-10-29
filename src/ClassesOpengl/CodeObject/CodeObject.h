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

class CodeObject: public ComponentBase
{
public:
	CodeObject(ShaderClass* shader, ModelHandler* model);
	~CodeObject();
    void Implementation() override;
	virtual void StartCodeEngine();
	virtual void UpdateCodeEngine();
	void AddParent(CodeObject* parent);	
	//material
	template<typename T>
	T* AddComponent();

	template<typename T>
	T* GetComponent();
	int id;
	std::string name;
	bool showed;
	Transform* transform;
	std::vector<ComponentBase*> components;
	std::vector<CodeObject*> parents;
	ShaderClass* shader;
	ModelHandler* modelHandlerController;
	std::shared_ptr<Material> material;
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
			//std::cout << "Component found" << std::endl;
			return derivedComponent;
		}
	}
	//std::cout << "Component Not found" << std::endl;

	return nullptr;
}
template<typename T>
T* CodeObject::AddComponent()
{
	T* component = new T();
	components.push_back(component);

	return component;
}
