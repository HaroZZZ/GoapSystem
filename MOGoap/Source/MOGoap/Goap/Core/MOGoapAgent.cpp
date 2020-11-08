#include "MOGoapAgent.h"
#include "MOGoapGoal.h"
#include <Kismet/KismetSystemLibrary.h>
#include "MOGoapAction.h"
#include "MOGoapState.h"
#include "../Goals/IdleGoal.h"
#include "../Actions/IdleAction.h"
#include "../Goals/MoveGoal.h"
#include "../Actions/MoveAction.h"
#include "../Actions/GatherResourceAction.h"
#include "../../MOGoapAI.h"
#include "../Goals/GetItemGoal.h"
#include "../Goals/KillEnemyGoal.h"
#include "../Actions/AttackAction.h"
#include "../Actions/ClimbAction.h"
#include "../Actions/ClimbNeedErectAction.h"
#include "../Actions/ErectAction.h"

MOGoapAgent::MOGoapAgent()
{
	Init();
}

MOGoapAgent::MOGoapAgent(AMOGoapAI* owner)
{
	Owner = owner;

	Init();
}

MOGoapAgent::~MOGoapAgent()
{
	delete CurrentGoapPlanWorker;
	CurrentGoapPlanWorker = nullptr;
}

void MOGoapAgent::Init()
{
	StartingPlan = nullptr;
	CurrentGoal = nullptr;
	possibleGoalsDirty = false;
	CurrentActionState = new MOGoapActionState{};
	Memory = new MOGoapMemory();
	//Memory->state->Set("needPower",true);
	CalculateNewGoalOnStart = true;
	interruptOnNextTransition = false;
	BlackListGoalOnFailure = false;
	CurrentGoapPlanWorker = nullptr;

	ActionEndFunc = std::bind(&MOGoapAgent::WarnActionEnd, this, std::placeholders::_1);
	ActionFailFunc = std::bind(&MOGoapAgent::WarnActionFailure, this, std::placeholders::_1);

	goalEndFunc = std::bind(&MOGoapAgent::WarnGoalEnd, this, std::placeholders::_1);
	goalPossibleFunc = std::bind(&MOGoapAgent::WarnPossibleGoal, this, std::placeholders::_1);

	planDoneFunc = std::bind(&MOGoapAgent::OnDonePlanning, this, std::placeholders::_1);

	//add goal
	MOGoapGoal* g1 = new IdleGoal();
	Goals.push_back(g1);
	//MOGoapGoal* g2 = new MoveGoal();
	//Goals.push_back(g2);
	MOGoapGoal* g3 = new GetItemGoal();
	Goals.push_back(g3);
	MOGoapGoal* g4 = new KillEnemyGoal();
	Goals.push_back(g4);


	MOGoapAction* a1 = new IdleAction();
	Actions.push_back(a1);
	MOGoapAction* a2 = new MoveAction();
	Actions.push_back(a2);
	MOGoapAction* a3 = new GatherResourceAction();
	Actions.push_back(a3);
	MOGoapAction* a4 = new AttackAction();
	Actions.push_back(a4);
	MOGoapAction* a5 = new ClimbAction();
	Actions.push_back(a5);
	MOGoapAction* a6 = new ClimbNeedErectAction();
	Actions.push_back(a6);
	MOGoapAction* a7 = new ErectAction();
	Actions.push_back(a7);
}

void MOGoapAgent::Start()
{
	if (CalculateNewGoalOnStart)
	{
		CalculateNewGoal(true);
	}
}

void MOGoapAgent::Update(float DeltaTime)
{
	if (CurrentActionState != nullptr && CurrentActionState->Action != nullptr)
	{
		CurrentActionState->Action->UpdateAction(DeltaTime);
	}

	//if (CurrentGoal != nullptr)
	//{
	//	CurrentGoal->UpdateGoal(DeltaTime);
	//}

	if (startedPlanning == false && CurrentActionState == nullptr)
	{
		CalculateNewGoal();
	}
}

void MOGoapAgent::UpdatePossibleGoals()
{
	possibleGoalsDirty = false;
	if (GoalBlackList.size() > 0)
	{
		PossibleGoals = std::vector<MOGoapGoal*>(Goals.size());
		for (auto& goal : Goals)
		{
			auto iter = GoalBlackList.find(goal->GetName());
			if (iter == GoalBlackList.end())
			{
				PossibleGoals.push_back(goal);
			}
			else if (iter->second < UKismetSystemLibrary::GetGameTimeInSeconds(Owner))
			{
				GoalBlackList.erase(goal->GetName());
				PossibleGoals.push_back(goal);
			}
		}
	}
	else
	{
		PossibleGoals = Goals;
	}
}

bool MOGoapAgent::CalculateNewGoal(bool forceStart /*= false*/)
{
	if (IsPlanning())
		return false;

	if (!forceStart && (UKismetSystemLibrary::GetGameTimeInSeconds(Owner) - lastCalculationTime <= CalculationDelay))
		return false;

	lastCalculationTime = UKismetSystemLibrary::GetGameTimeInSeconds(Owner);
	interruptOnNextTransition = false;
	UpdatePossibleGoals();
	startedPlanning = true;
	//plan
	CurrentGoapPlanWorker = MOGoapPlannerManager::Instance->Plan(this, BlackListGoalOnFailure ? CurrentGoal : nullptr, CurrentGoal != nullptr ? CurrentGoal->GetPlan() : nullptr, planDoneFunc);

	return true;
}

void MOGoapAgent::WarnActionEnd(MOGoapAction* thisAction)
{
	if (CurrentActionState->Action != nullptr && thisAction != CurrentActionState->Action)
		return;

	PushAction();
}

void MOGoapAgent::WarnActionFailure(MOGoapAction* thisAction)
{
	if (CurrentActionState->Action != nullptr && thisAction != CurrentActionState->Action)
		return;

	if (BlackListGoalOnFailure && CurrentGoal != nullptr)
		GoalBlackList[CurrentGoal->GetName()] = CurrentGoal->GetErrorDelay() + UKismetSystemLibrary::GetGameTimeInSeconds(Owner);

	CalculateNewGoal(true);
}

void MOGoapAgent::PushAction()
{
	if (interruptOnNextTransition)
	{
		CalculateNewGoal();
		return;
	}
	auto plan = CurrentGoal->GetPlan();
	if (!Owner->IsPendingKill())
	Owner->PrintActionNum(plan->size());
	//当前目标的任务列表执行完毕
	if (plan->size() == 0)
	{
		if (CurrentActionState != nullptr && CurrentActionState->Action != nullptr)
		{
			CurrentActionState->Action->Exit(CurrentActionState->Action);
			CurrentActionState = nullptr;
		}
		//Owner->PrintGoal(CurrentGoal->GetName().c_str());

		CalculateNewGoal();
	}
	else
	{
		auto previous = CurrentActionState;
		CurrentActionState = plan->front();
		plan->pop_front();
		MOGoapAction* next = nullptr;
		if (plan->size() > 0)
			next = plan->front()->Action;

		if (previous != nullptr && previous->Action != nullptr)
			previous->Action->Exit(CurrentActionState->Action);

		auto goalstate = CurrentGoal->GetGoalState();
		CurrentActionState->Action->Run(previous != nullptr ? previous->Action : nullptr,next, CurrentActionState->Settings, goalstate, ActionEndFunc, ActionFailFunc);
	}
}

void MOGoapAgent::WarnGoalEnd(MOGoapGoal* goal)
{
	if (*CurrentGoal != *goal)
		return;

	CalculateNewGoal();
}

void MOGoapAgent::WarnPossibleGoal(MOGoapGoal* goal)
{
	if (CurrentGoal != nullptr && goal->GetPriority() <= CurrentGoal->GetPriority())
		return;

	if (CurrentActionState->Action && !CurrentActionState->Action->IsInterruptable())
	{
		interruptOnNextTransition = true;
		CurrentActionState->Action->AskForInterruption();
	}
	else
		CalculateNewGoal();
}

void MOGoapAgent::OnDonePlanning(MOGoapGoal* newgoal)
{
	startedPlanning = false;

	CurrentGoapPlanWorker = nullptr;

	if (newgoal == nullptr)
	{
		return;
	}

	if (CurrentActionState != nullptr && CurrentActionState->Action != nullptr)
	{
		CurrentActionState->Action->Exit(nullptr);
	}
	CurrentGoal = newgoal;

	StartingPlan = CurrentGoal->GetPlan();

	if(!Owner->IsPendingKill())
	Owner->PrintGoal(CurrentGoal->GetName().c_str());
	CurrentGoal->Run(goalEndFunc);

	PushAction();
}

void MOGoapAgent::RefreshMemory()
{
	Memory = new MOGoapMemory();

}

void MOGoapAgent::RefreshGoalsSet()
{
	possibleGoalsDirty = true;
}

void MOGoapAgent::RefreshActionsSet()
{
	Actions.clear();
}

