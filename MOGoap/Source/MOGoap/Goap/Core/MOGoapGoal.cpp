#include "MOGoapGoal.h"
#include "MOGoapPlanner.h"
#include "MOGoapAgent.h"
#include "../../MOGoapAI.h"

MOGoapGoal::MOGoapGoal()
{
	GoalName = "GenericGoal";
	Priority = 1;
	ErrorDelay = 0.5f;
	WarnPossibleGoal = true;
	GoalState = MOGoapState::Instantiate();
	Planner = nullptr;
	GoalStart = false;
}

MOGoapGoal::MOGoapGoal(const MOGoapGoal& rhs)
{
	GoalName = rhs.GetName();
	Priority = rhs.GetPriority();
	ErrorDelay = 0.5f;
	WarnPossibleGoal = true;
}

void MOGoapGoal::Run(PlannerDoneFunc callBack)
{
	GoalEnd = callBack;
	GoalStart = true;
}

void MOGoapGoal::Precalculations(MOGoapPlanner* goapPlanner)
{
	Planner = goapPlanner;
}

bool MOGoapGoal::IsGoalPossible()
{
	return true;
}

void MOGoapGoal::UpdateGoal(float DeltaTime)
{
	if (Plans.size() == 0 && GoalEnd != nullptr && GoalStart)
	{
		//Planner->GetCurrentAgent()->GetOwner()->PrintCurrrentGoal(GetName().c_str());
		GoalEnd(this);
		GoalStart = false;
	}
}

bool MOGoapGoal::operator==(const MOGoapGoal& rhs)
{
	if (GoalName == rhs.GetName() && Priority == rhs.GetPriority() && GoalState == rhs.GetGoalState())
		return true;

	return false;
}

bool MOGoapGoal::operator!=(const MOGoapGoal& rhs)
{
	return !(*this == rhs);
}

MOGoapGoal::~MOGoapGoal()
{

}

MOGoapGoal& MOGoapGoal::operator=(const MOGoapGoal& rhs)
{
	GoalName = rhs.GetName();
	Priority = rhs.GetPriority();
	ErrorDelay = 0.5f;
	WarnPossibleGoal = true;

	return *this;
}
