#include "GetItemGoal.h"

GetItemGoal::GetItemGoal():MOGoapGoal()
{
	ItemID = "Power";
	GoalState->Set("need" + ItemID, true);
	GoalName = "GetItem";
	Priority = 2;
}

void GetItemGoal::Run(PlannerDoneFunc callBack)
{
	MOGoapGoal::Run(callBack);
}

bool GetItemGoal::IsGoalPossible()
{
	return true;
}
