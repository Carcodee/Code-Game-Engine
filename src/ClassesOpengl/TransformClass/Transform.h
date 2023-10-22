#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../CodeObject/ComponentBase/ComponentBase.h"

class Transform: public ComponentBase
{
	public:
	Transform();
	
	void Implementation () override;

	~Transform();
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);
	glm::vec3* position;
	glm::vec3* rotation;
	glm::vec3* scale;
	private:
	glm::mat4 localModelMatrix;

};

