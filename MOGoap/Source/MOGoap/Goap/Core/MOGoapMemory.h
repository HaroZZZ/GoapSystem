#pragma once
#include "MOGoapState.h"

class MOGoapMemory
{
public:
	MOGoapState* state;

	MOGoapMemory() 
	{
		state = MOGoapState::Instantiate();
	}


	MOGoapState* GetWorldState()
	{
		return state;
	}

	~MOGoapMemory()
	{
		state->Recycle();
	}
};
