#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MOGoapPlannerManager.h"
#include "MOGoapMemory.h"


class AMOGoapAI;

class MOGoapAgent
{
public:
	MOGoapAgent();

	MOGoapAgent(AMOGoapAI* owner);

	virtual ~MOGoapAgent();

	void Init();

	MOGoapActionState* CurrentActionState;
protected:

	AMOGoapAI* Owner;

	float CalculationDelay;
	bool CalculateNewGoalOnStart;
	float lastCalculationTime;

	bool BlackListGoalOnFailure;

	std::vector<MOGoapGoal*> Goals;
	std::vector<MOGoapAction*> Actions;

	MOGoapMemory* Memory;

	MOGoapGoal* CurrentGoal;

	std::map<std::string, float> GoalBlackList;

	std::vector<MOGoapGoal*> PossibleGoals;
	bool possibleGoalsDirty;
	std::list<MOGoapActionState*>* StartingPlan;

	bool interruptOnNextTransition;

	bool startedPlanning;

	MOGoapPlanWork* CurrentGoapPlanWorker;

	ActionCallBack ActionEndFunc;
	ActionCallBack ActionFailFunc;

	PlannerDoneFunc goalEndFunc;
	PlannerDoneFunc goalPossibleFunc;

	PlannerDoneFunc planDoneFunc;
public:
	void Start();

	void Update(float DeltaTime);

	void UpdatePossibleGoals();

	bool CalculateNewGoal(bool forceStart = false);

	void WarnActionEnd(MOGoapAction* thisAction);

	void WarnActionFailure(MOGoapAction* thisAction);

	void PushAction();

	void WarnGoalEnd(MOGoapGoal* goal);

	void WarnPossibleGoal(MOGoapGoal* goal);

	void OnDonePlanning(MOGoapGoal* goal);

	void RefreshMemory();

	void RefreshGoalsSet();

	void RefreshActionsSet();

	void AddGoal(MOGoapGoal* newgoal)
	{
		Goals.push_back(newgoal);
	}

	bool IsPlanning()
	{
		return startedPlanning && CurrentGoapPlanWorker != nullptr && CurrentGoapPlanWorker->NewGoal == nullptr;
	}

	AMOGoapAI* GetOwner() { return Owner; }

	std::list<MOGoapActionState*>* GetStartingPlan()
	{
		return StartingPlan;
	}

	std::vector<MOGoapGoal*>& GetGoalsSet()
	{
		if (possibleGoalsDirty)
			UpdatePossibleGoals();
		return PossibleGoals;
	}

	std::vector<MOGoapAction*>& GetActionsSet()
	{
		return Actions;
	}

	MOGoapGoal* GetCurrentGoal()
	{
		if (CurrentGoal != nullptr)
			return CurrentGoal;
		else
			return nullptr;
	}

	MOGoapMemory* GetMemory()
	{
		return Memory;
	}
};