#include "IdleAction.h"
#include "GameFramework/Character.h"
#include "../Core/MOGoapAgent.h"
#include <Engine/Engine.h>
#include "../../MOGoapAI.h"

IdleAction::IdleAction():MOGoapAction()
{
	ActionName = "IdleAction";

	effects->Set("needIdle", true);

	DelayTime = 5.0f;
}

std::vector<MOGoapState*> IdleAction::GetSettings(const MOGoapActionStackData& stackData)
{
	//ActionSettings->Set("GotoPoint", true);
	//SettingsList.push_back(ActionSettings->Clone());

	return SettingsList;
}

void IdleAction::Run(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState, ActionCallBack done /*= nullptr*/, ActionCallBack fail /*= nullptr*/)
{
	MOGoapAction::Run(previousAction, nextAction, settings, goalState, done);

	if (Agent != nullptr)
	{
		Agent->GetOwner()->PrintAction(ActionName.c_str());
	}
}


MOGoapState* IdleAction::GetPreconditions(MOGoapActionStackData& stackData)
{
	return preconditions;
}

MOGoapState* IdleAction::GetEffects(MOGoapActionStackData& stackData)
{
	return effects;
}

bool IdleAction::CheckProceduralCondition(MOGoapActionStackData& stackData)
{
	return true;
}

void IdleAction::UpdateAction(float DeltaTime)
{
	TotalTime += DeltaTime;
	if (TotalTime >= DelayTime)
	{
		DoneCallBack(this);
		TotalTime = 0.0f;
	}
}
