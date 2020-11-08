#pragma once
#include "../Core/MOGoapGoal.h"

class KillEnemyGoal : public MOGoapGoal
{
public:
	KillEnemyGoal();

	virtual void Run(PlannerDoneFunc callBack) override;

	virtual bool IsGoalPossible()override;
};