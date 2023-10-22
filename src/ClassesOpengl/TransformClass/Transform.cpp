#include "Transform.h"

Transform::Transform()
{
	position = new glm::vec3(0.0f, 0.0f, 0.0f);
	rotation =new glm::vec3(0.0f, 0.0f, 0.0f);
	scale =new glm::vec3(1.0f, 1.0f, 1.0f);

	localModelMatrix = glm::mat4(1.0f);
	localModelMatrix= glm::translate(localModelMatrix, *position);
	localModelMatrix = glm::rotate(localModelMatrix, glm::radians(rotation->x), glm::vec3(1.0f, 0.0f, 0.0f));
	localModelMatrix = glm::rotate(localModelMatrix, glm::radians(rotation->y), glm::vec3(0.0f, 1.0f, 0.0f));
	localModelMatrix = glm::rotate(localModelMatrix, glm::radians(rotation->z), glm::vec3(0.0f, 0.0f, 1.0f));
	localModelMatrix = glm::scale(localModelMatrix, *scale);
}

void Transform::Implementation()
{

}

Transform::~Transform()
{
	delete position;
	delete rotation;
	delete scale;
}


void Transform::SetPosition(glm::vec3 newPosition)
{
	*position=newPosition;
}

void Transform::SetRotation(glm::vec3 newRotation)
{
	*rotation = newRotation;
}

void Transform::SetScale(glm::vec3 newScale)
{
	*scale = newScale;
}


