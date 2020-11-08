#include "MOGoapAction.h"
#include "MOGoapAgent.h"
#include "../../MOGoapManager.h"


MOGoapAction::MOGoapAction()
{
	ActionName = "DefaultAction";
	ActionSettings = MOGoapState::Instantiate();
	effects = MOGoapState::Instantiate();
	preconditions = MOGoapState::Instantiate();
	PreviousAction = nullptr;
	NextAction = nullptr;
	Agent = nullptr;
	cost = 0;
	DelayTime = 0;
	TotalTime = 0;
}

MOGoapAction::~MOGoapAction()
{

}

std::vector<MOGoapState*> MOGoapAction::GetSettings(const MOGoapActionStackData& stackData)
{

	return std::vector<MOGoapState*>{ActionSettings};
}

void MOGoapAction::Run(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState, ActionCallBack done /*= nullptr*/, ActionCallBack fail /*= nullptr*/)
{
	PreviousAction = previousAction;
	NextAction = nextAction;

	DoneCallBack = done;
	FailCallBack = fail;

	ActionSettings = settings;
}

void MOGoapAction::PlanEnter(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState)
{

}

void MOGoapAction::PlanExit(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState)
{

}

void MOGoapAction::Exit(MOGoapAction* nextAction)
{

}

void MOGoapAction::UpdateAction(float DeltaTime)
{

}

std::string MOGoapAction::GetName()
{
	return ActionName;
}

bool MOGoapAction::IsActive()
{
	return true;
}

void MOGoapAction::ActionDone()
{
	DoneCallBack(this);
}

MOGoapState* MOGoapAction::GetPreconditions(MOGoapActionStackData& stackData)
{
	return preconditions;
}

MOGoapState* MOGoapAction::GetEffects(MOGoapActionStackData& stackData)
{
	return effects;
}

bool MOGoapAction::CheckProceduralCondition(MOGoapActionStackData& stackData)
{
	return true;
}

float MOGoapAction::GetCost(MOGoapActionStackData& stackData)
{
	return cost;
}

void MOGoapAction::Precalculations(const MOGoapActionStackData& stackData)
{
	Agent = stackData.agent;
}

bool MOGoapAction::operator==(const MOGoapAction& rhs) const
{
	return false;
}

