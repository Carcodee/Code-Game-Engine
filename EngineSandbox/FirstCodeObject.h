#pragma once
#include <iostream>
#include <string>
#include "../src/ClassesOpengl/CodeObject/CodeObject.h"
class FirstCodeObject: public CodeObject
{
	public:
		FirstCodeObject(std::string name, int id,ShaderClass* modelShader) :CodeObject(name, id, modelShader) {};
		~FirstCodeObject();
    void StartCodeEngine() override;
	void UpdateCodeEngine() override;

	Model* model;
};

