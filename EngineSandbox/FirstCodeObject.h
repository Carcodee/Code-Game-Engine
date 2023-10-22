#pragma once
#include <iostream>
#include <string>
#include "../src/ClassesOpengl/CodeObject/CodeObject.h"
class FirstCodeObject: public CodeObject
{
	public:
		FirstCodeObject(ShaderClass* modelShader,ModelHandler* model) :CodeObject(modelShader, model) {};
		~FirstCodeObject();
    void StartCodeEngine() override;
	void UpdateCodeEngine() override;

	Model* model;
};

