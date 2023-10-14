#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../src/ClassesOpengl/Mateial/Material.h"
#include "../src/Model/Model.h"
#include "../src/ClassesOpengl/TransformClass/Transform.h"
class CodeObject
{
public:
	CodeObject();
	~CodeObject();

	void StartCodeEngine();
	void UpdateCodeEngine();
	
	
	std::string name;
	std::vector<Material> materials;
	Transform transform;
	Model model;
	std::vector<CodeObject>* parents;
};

