#pragma once
#include "../Core/MOGoapGoal.h"

class IdleGoal : public MOGoapGoal
{
public:
	IdleGoal();

	virtual void Run(PlannerDoneFunc callBack) override;

	virtual bool IsGoalPossible()override;
};