#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../CodeObject/ComponentBase/ComponentBase.h"
#include "../src/GameEngineActions/IUndoable.h"
#include <iostream>
#include <stack>

struct UndoRedoPos
{
	glm::vec3 undoPos;
	glm::vec3 redoPos;
};
class Transform: public ComponentBase, public IUndoable
{


	public:
	Transform();
	
	void Implementation () override;

	~Transform();
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);
	glm::mat4 GetLocalModelMatrix();
	void SetLocalModelMatrix(glm::mat4 matrix);

	void SetValueUndoRedoPos();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void Undo() override;
	void Redo() override;

	glm::vec3 positionBeforeMove;
	glm::vec3 rotationBeforeMove;
	glm::vec3 scaleBeforeMove;

	glm::vec3 positionAfterMove;
	glm::vec3 rotationAfterMove;
	glm::vec3 scaleAfterMove;
	private:
	glm::mat4 localModelMatrix;
	std::stack<UndoRedoPos*> undoPositionStack;
};

