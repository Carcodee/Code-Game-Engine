#pragma once
#include <iostream>
#include <stack>
#include "../src/GameEngineActions/IUndoable.h"

class EventSystem
{
	public:
	EventSystem();

	void OnEventFinished();
	void OnEvent();
	void UndoAction();

	void SetUndoStack(std::stack<IUndoable*> undoStack);

	bool actionFinished = true;
	bool isUnduKeyPressed = false;
	std::stack<IUndoable*> undoStack;

	private:

};

