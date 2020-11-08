#include "MOGoapPlannerManager.h"
#include "MOGoapAgent.h"

MOGoapPlannerManager* MOGoapPlannerManager::Instance;

MOGoapPlannerManager::MOGoapPlannerManager()
{
	DoneWorks = std::vector<MOGoapPlanWork*>{};

	Planners = std::vector<MOGoapPlannerThread*>{};

	PlannerSettings = new MOGoapPlannerSettings();
}

std::queue<MOGoapPlanWork*> MOGoapPlannerThread::WorkQueue;

void MOGoapPlannerManager::Start()
{
	if (Instance == nullptr)
		Instance = this;

	OnDonePlanFunc donefunc = std::bind(&MOGoapPlannerManager::OnDonePlan, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	Planners.clear();

	MOGoapPlannerThread* planer = new MOGoapPlannerThread(PlannerSettings, donefunc);
	Planners.push_back(planer);
}

void MOGoapPlannerManager::Update()
{
	if (DoneWorks.size() > 0)
	{
		for (auto& work : DoneWorks)
		{
			work->CallBack(work->NewGoal);
		}
		DoneWorks.clear();
	}
	for (auto& planner : Planners)
	{
		planner->CheckWorkers();
	}
}

void MOGoapPlannerManager::OnDonePlan(MOGoapPlannerThread* plannerThread, MOGoapPlanWork* work, MOGoapGoal* newgoal)
{
	work->NewGoal = newgoal;
	DoneWorks.push_back(work);
}

MOGoapPlanWork* MOGoapPlannerManager::Plan(MOGoapAgent* agent, MOGoapGoal* blacklistGoal, std::list<MOGoapActionState*>* currentPlan, PlannerDoneFunc callBack)
{
	MOGoapPlanWork* work = new MOGoapPlanWork(agent,blacklistGoal,currentPlan,callBack);

	MOGoapPlannerThread::WorkQueue.push(work);
	return work;
}

MOGoapPlannerManager* MOGoapPlannerManager::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = new MOGoapPlannerManager();
	}

	return Instance;
}

MOGoapPlannerThread::MOGoapPlannerThread(MOGoapPlannerSettings* plannerSettings, OnDonePlanFunc ondone)
{
	Planner = new MOGoapPlanner(plannerSettings);
	onDonePlan = ondone;
}

void MOGoapPlannerThread::Update()
{
	while (isRunning)
	{
		CheckWorkers();
	}
}

void MOGoapPlannerThread::CheckWorkers()
{
	if (WorkQueue.size() > 0)
	{
		MOGoapPlanWork* checkWork = nullptr;

		checkWork = WorkQueue.front();
		WorkQueue.pop();

		MOGoapPlannerThread* pt = this;

		Planner->Plan(checkWork->Agent, checkWork->BlacklistGoal, checkWork->Actions, [&pt, checkWork](MOGoapGoal* goal) {pt->onDonePlan(pt,checkWork,goal); });
	}
}
