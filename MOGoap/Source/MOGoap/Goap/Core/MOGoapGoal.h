#pragma once
#include "CoreMinimal.h"
#include "MOGoapAction.h"
#include <queue>
#include "MOGoapState.h"

class MOGoapGoal;
class MOGoapPlanner;

using PlannerDoneFunc = std::function<void(MOGoapGoal*)>;

class MOGoapGoal
{
public:
	MOGoapGoal();

	MOGoapGoal(const MOGoapGoal& rhs);

	std::list<MOGoapActionState*>* GetPlan() { return &Plans; }

	std::string GetName() const { return GoalName; }

	virtual void Run(PlannerDoneFunc callBack);

	virtual void Precalculations(MOGoapPlanner* goapPlanner);

	virtual bool IsGoalPossible();

	virtual void UpdateGoal(float DeltaTime);

	MOGoapState* GetGoalState() const{
		return GoalState;
	}

	float GetPriority() const { return Priority; }
	
	float GetErrorDelay()const { return ErrorDelay; }

	void SetPlan(std::list<MOGoapActionState*> path) { Plans = path; }

	MOGoapGoal& operator=(const MOGoapGoal& rhs);
	
	bool operator==(const MOGoapGoal& rhs);

	bool operator!=(const MOGoapGoal& rhs);

	virtual ~MOGoapGoal();

protected:
	std::string GoalName;

	float Priority;

	float ErrorDelay;

	bool WarnPossibleGoal;

	bool GoalStart;

	MOGoapState* GoalState;

	std::list<MOGoapActionState*> Plans;

	MOGoapPlanner* Planner;

	PlannerDoneFunc GoalEnd;
	//planner
};