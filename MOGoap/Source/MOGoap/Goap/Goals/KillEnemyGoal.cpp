#include "KillEnemyGoal.h"

KillEnemyGoal::KillEnemyGoal():MOGoapGoal()
{
	GoalState->Set("enemyDie", true);
	GoalName = "KillEnemyGoal";
	Priority = 3;
}

void KillEnemyGoal::Run(PlannerDoneFunc callBack)
{
	MOGoapGoal::Run(callBack);
}

bool KillEnemyGoal::IsGoalPossible()
{
	return true;
}
