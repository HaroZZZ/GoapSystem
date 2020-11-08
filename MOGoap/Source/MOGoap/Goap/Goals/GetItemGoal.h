#pragma once
#include "../Core/MOGoapGoal.h"

class GetItemGoal : public MOGoapGoal
{
public:
	GetItemGoal();

	virtual void Run(PlannerDoneFunc callBack) override;

	virtual bool IsGoalPossible()override;

	std::string ItemID;
};