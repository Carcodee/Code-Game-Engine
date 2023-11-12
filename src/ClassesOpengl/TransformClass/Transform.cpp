#include "Transform.h"

Transform::Transform()
{
	position =glm::vec3(0.0f, 0.0f, 0.0f);
	rotation =glm::vec3(0.0f, 0.0f, 0.0f);
	scale =glm::vec3(1.0f, 1.0f, 1.0f);

	localModelMatrix = glm::mat4(1.0f);
	localModelMatrix= glm::translate(localModelMatrix, position);
	localModelMatrix = glm::rotate(localModelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	localModelMatrix = glm::rotate(localModelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	localModelMatrix = glm::rotate(localModelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	localModelMatrix = glm::scale(localModelMatrix, scale);
	this->SetValueUndoRedoPos();
}

void Transform::Implementation()
{

}

Transform::~Transform()
{

}


void Transform::SetPosition(glm::vec3 newPosition)
{
	position=newPosition;
}

void Transform::SetRotation(glm::vec3 newRotation)
{
	rotation = newRotation;
}

void Transform::SetScale(glm::vec3 newScale)
{
	scale = newScale;
}

glm::mat4 Transform::GetLocalModelMatrix()
{
	return localModelMatrix;
}

void Transform::SetLocalModelMatrix(glm::mat4 matrix)
{
	position.x=  localModelMatrix[3][0];
	position.y = localModelMatrix[3][1];
	position.z = localModelMatrix[3][2];

	localModelMatrix = matrix;
}

void Transform::SetValueUndoRedoPos()
{
	positionBeforeMove = positionAfterMove;
	positionAfterMove = position;
	UndoRedoPos* undoRedoPos = new UndoRedoPos();
	undoRedoPos->undoPos = positionBeforeMove;
	undoRedoPos->redoPos = positionAfterMove;
	undoPositionStack.push(undoRedoPos);
	std::cout << "undo stack size: " << undoPositionStack.size() << std::endl;
	std::cout << undoRedoPos->undoPos.x << std::endl;
	std::cout << undoRedoPos->redoPos.x << std::endl;

	
}




void Transform::Undo()
{

	this->SetPosition(undoPositionStack.top()->undoPos);
	this->SetRotation(rotationBeforeMove);
	this->SetScale(scaleBeforeMove);
	localModelMatrix = glm::translate(glm::mat4(1.0f), undoPositionStack.top()->undoPos);
	std::cout << "//////////UNDO ACTION/////////// " << undoPositionStack.size() << std::endl;
	std::cout << "undo stack size: " << undoPositionStack.size() << std::endl;
	std::cout << undoPositionStack.top()->redoPos.x << std::endl;
	std::cout << "//////////////////////////////// " << undoPositionStack.size() << std::endl;

	undoPositionStack.pop();

}

void Transform::Redo()
{
	this->SetPosition(positionAfterMove);
	this->SetRotation(rotationAfterMove);
	this->SetScale(scaleAfterMove);


}
