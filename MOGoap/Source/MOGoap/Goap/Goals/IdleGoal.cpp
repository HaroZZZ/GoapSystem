#include "IdleGoal.h"

IdleGoal::IdleGoal():MOGoapGoal()
{
	GoalState->Set("needIdle", true);
	GoalName = "IdleGoal";
	Priority = 1;
}

void IdleGoal::Run(PlannerDoneFunc callBack)
{
	MOGoapGoal::Run(callBack);
}

bool IdleGoal::IsGoalPossible()
{
	return true;
}
