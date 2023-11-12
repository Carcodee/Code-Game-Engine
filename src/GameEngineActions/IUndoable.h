#pragma once
class IUndoable
{
	public:
	virtual void Undo()=0;
	virtual void Redo()=0;

};

