#include "MoveGoal.h"

MoveGoal::MoveGoal():MOGoapGoal()
{
	GoalState->Set("canMove", true);
	GoalName = "MoveGoal";
	Priority = 2;
	
}

void MoveGoal::Run(PlannerDoneFunc callBack)
{
	MOGoapGoal::Run(callBack);
}

bool MoveGoal::IsGoalPossible()
{
	return true;
}
