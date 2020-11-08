#pragma once
#include "CoreMinimal.h"
#include "MOGoapAction.h"
#include "MOGoapGoal.h"
#include "MOGoapPlannerSettings.h"
#include "MOGoapPlanner.h"

class MOGoapAgent;

struct MOGoapPlanWork
{
	MOGoapAgent* Agent;
	MOGoapGoal* BlacklistGoal;
	std::list<MOGoapActionState*>* Actions;

	PlannerDoneFunc CallBack;

	MOGoapGoal* NewGoal;

	MOGoapPlanWork() {	}

	MOGoapPlanWork(MOGoapAgent* agent, MOGoapGoal* blacklistGoal, std::list<MOGoapActionState*>* actions, PlannerDoneFunc callback)
	{
		Agent = agent;
		BlacklistGoal = blacklistGoal;
		Actions = actions;
		CallBack = callback;
		NewGoal = nullptr;
	}
};

class MOGoapPlannerThread;

using OnDonePlanFunc = std::function<void(MOGoapPlannerThread*, MOGoapPlanWork*, MOGoapGoal*)>;

class MOGoapPlannerThread
{
private:
	MOGoapPlanner* Planner;

	bool isRunning;

public:
	MOGoapPlannerThread(MOGoapPlannerSettings* plannerSettings, OnDonePlanFunc ondone);

	~MOGoapPlannerThread()
	{
		delete Planner;
		Planner = nullptr;
	}

	void Update();

	void CheckWorkers();

	OnDonePlanFunc onDonePlan;

	static std::queue<MOGoapPlanWork*> WorkQueue;
};


class MOGoapPlannerManager
{
public:
	static MOGoapPlannerManager* Instance;

	MOGoapPlannerManager();

	void Start();

	void OnDestroy();

	void Update();

	void OnDonePlan(MOGoapPlannerThread* plannerThread,MOGoapPlanWork* work,MOGoapGoal* newgoal);

	MOGoapPlanWork* Plan(MOGoapAgent* agent, MOGoapGoal* blacklistGoal, std::list<MOGoapActionState*>* currentPlan, PlannerDoneFunc callBack);

	static MOGoapPlannerManager* GetInstance();
private:
	std::vector<MOGoapPlanWork*> DoneWorks;

	std::vector<MOGoapPlannerThread*> Planners;

	MOGoapPlannerSettings* PlannerSettings;
};
