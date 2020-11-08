#pragma once
#include "../Core/MOGoapGoal.h"

class MoveGoal : public MOGoapGoal
{
public:
	MoveGoal();

	virtual void Run(PlannerDoneFunc callBack) override;

	virtual bool IsGoalPossible()override;
};