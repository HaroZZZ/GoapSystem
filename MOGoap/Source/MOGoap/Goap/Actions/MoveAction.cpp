#include "MoveAction.h"
#include "GameFramework/Character.h"
#include "../Core/MOGoapAgent.h"
#include <Engine/Engine.h>
#include "../../MOGoapAI.h"

MoveAction::MoveAction():MOGoapAction()
{
	ActionName = "MoveAction";

	//effects->Set("canMove", true);

	//ActionSettings->Set("GotoPoint", true);

	DelayTime = 1.0f;
}

std::vector<MOGoapState*> MoveAction::GetSettings(const MOGoapActionStackData& stackData)
{
	ActionSettings->Clear();
	if (stackData.goalState->HasKey("isAtPosition"))
	{
		ActionSettings->Set("objectPosition", stackData.goalState->Get("isAtPosition"));
		return MOGoapAction::GetSettings(stackData);
	}

	return SettingsList;
}

void MoveAction::Run(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState, ActionCallBack done /*= nullptr*/, ActionCallBack fail /*= nullptr*/)
{
	MOGoapAction::Run(previousAction, nextAction, settings, goalState, done);

	Agent->GetOwner()->MoveTo(settings->GetValue<FVector>("objectPosition"),this);
	Agent->GetOwner()->PrintPosition(settings->GetValue<FVector>("objectPosition"));

	if (Agent != nullptr)
	{
		Agent->GetOwner()->PrintAction(ActionName.c_str());
	}
}

MOGoapState* MoveAction::GetPreconditions(MOGoapActionStackData& stackData)
{

	return preconditions;
}

MOGoapState* MoveAction::GetEffects(MOGoapActionStackData& stackData)
{
	if (stackData.settings->HasKey("objectPosition"))
	{
		effects->Set("isAtPosition", stackData.settings->Get("objectPosition"));
	}
	else
	{
		effects->Clear();
	}

	return effects;
}

bool MoveAction::CheckProceduralCondition(MOGoapActionStackData& stackData)
{
	return true;
}

void MoveAction::UpdateAction(float DeltaTime)
{

}
