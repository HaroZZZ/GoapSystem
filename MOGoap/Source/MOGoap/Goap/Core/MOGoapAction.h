#pragma once
#include "MOGoapState.h"
#include <list>
#include "CoreMinimal.h"
#include <functional>
#include "UObject/NoExportTypes.h"

class MOGoapAgent;
class MOGoapAction;

using ActionCallBack = std::function<void(MOGoapAction*)>;

struct MOGoapActionStackData
{
	MOGoapState* currentState;
	MOGoapState* goalState;
	MOGoapAgent* agent;
	MOGoapAction* next;
	MOGoapState* settings;
};

class MOGoapAction 
{
protected:
	float cost;
	MOGoapAction* PreviousAction;
	MOGoapAction* NextAction;
	ActionCallBack DoneCallBack;
	ActionCallBack FailCallBack;

	bool interruptWhenPossible;
	//代理
	MOGoapAgent* Agent;

	float DelayTime;
	float TotalTime;

	//std::string TargetID;

protected:
	std::string ActionName;
	//前置条件
	MOGoapState* preconditions;
	//影响
	MOGoapState* effects;
	//设置
	MOGoapState* ActionSettings;
public:
	MOGoapAction();

	virtual ~MOGoapAction();

	/***************************************************************
		// this should return current's action calculated parameter, will be added to the run method
		// userful for dynamic actions, for example a GoTo action can save some informations (wanted position)
		// while being chosen from the planner, we save this information and give it back when we run the method
		// most of actions would return a single item list, but more complex could return many items
	***************************************************************/
	virtual std::vector<MOGoapState*> GetSettings(const MOGoapActionStackData& stackData);

	virtual void Run(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState, ActionCallBack done = nullptr, ActionCallBack fail = nullptr);
	/***************************************************************
	     @brief    Called when the action has been added inside a running Plan
	***************************************************************/
	virtual void PlanEnter(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState);
	/***************************************************************
	     @brief    Called when the plan, which had this action, has either failed or completed
	***************************************************************/
	virtual void PlanExit(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState);

	virtual void Exit(MOGoapAction* nextAction);

	// MUST BE IMPLEMENTED AS THREAD SAFE
	virtual void UpdateAction(float DeltaTime);

	virtual MOGoapState* GetPreconditions(MOGoapActionStackData& stackData);

	virtual MOGoapState* GetEffects(MOGoapActionStackData& stackData);

	virtual bool CheckProceduralCondition(MOGoapActionStackData& stackData);

	virtual float GetCost(MOGoapActionStackData& stackData);

	virtual void Precalculations(const MOGoapActionStackData& stackData);

	std::string GetName();

	bool IsActive();

	void ActionDone();

	inline virtual bool IsInterruptable(){ return true; }

	inline virtual void AskForInterruption()
	{
		interruptWhenPossible = true;
	}

	bool operator==(const MOGoapAction& rhs) const;
};


class MOGoapActionState
{
public:
	MOGoapActionState()
	{
		Action = nullptr;
		Settings = MOGoapState::Instantiate();
	}
	MOGoapActionState(MOGoapAction* action, MOGoapState* settings)
	{
		Action = action;
		Settings = settings;
	}

	void Init()
	{
		Action = nullptr;

		//Settings.Init();
	}

	MOGoapAction* Action;
	MOGoapState* Settings;
};