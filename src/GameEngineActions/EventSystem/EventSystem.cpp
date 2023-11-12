#include "EventSystem.h"

EventSystem::EventSystem()
{

}

void EventSystem::OnEventFinished()
{
	this->actionFinished = true;
}

void EventSystem::OnEvent()
{
	this->actionFinished = false;
}

void EventSystem::UndoAction()
{
	//prevents removving from empty stack
	if (undoStack.size()>0)
	{
		std::cout << "undo" << std::endl;
		undoStack.top()->Undo();
		undoStack.pop();
	}

}

void EventSystem::SetUndoStack(std::stack<IUndoable*> undoStack)
{
	this->undoStack = undoStack;
}
