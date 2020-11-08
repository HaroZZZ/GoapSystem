#include "ErectAction.h"
#include "GameFramework/Character.h"
#include "../Core/MOGoapAgent.h"
#include <Engine/Engine.h>
#include "../../MOGoapAI.h"
#include "../../PrimitiveResource.h"
#include "../../MOGoapManager.h"
#include "../../EntryPoint.h"

ErectAction::ErectAction():MOGoapAction()
{
	ActionName = "ErectAction";

	effects->Set("hasErect", true);

	DelayTime = 5.0f;

	preconditions->Set("hasLadder", true);
}

std::vector<MOGoapState*> ErectAction::GetSettings(const MOGoapActionStackData& stackData)
{
	ActionSettings->Clear();
	SettingsList.clear();

	if (stackData.currentState->HasKey("EntryPoint"))
	{
		AEntryPoint* entry = Cast<AEntryPoint>(stackData.currentState->GetValue<AActor*>("EntryPoint"));

		ActionSettings->Set("EntryPointPos", entry->GetActorLocation());
		SettingsList.push_back(ActionSettings->Clone());
	}

	
	return SettingsList;
}

void ErectAction::Run(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState, ActionCallBack done /*= nullptr*/, ActionCallBack fail /*= nullptr*/)
{
	MOGoapAction::Run(previousAction, nextAction, settings, goalState, done);

	if (Agent != nullptr)
	{
		Agent->GetOwner()->PrintAction(ActionName.c_str());
	}
}


MOGoapState* ErectAction::GetPreconditions(MOGoapActionStackData& stackData)
{
	if (stackData.settings != nullptr && stackData.settings->HasKey("EntryPointPos") && stackData.goalState->HasKey("hasErect") && stackData.goalState != nullptr)
	{
		preconditions->Set("isAtPosition", stackData.settings->GetValue<FVector>("EntryPointPos"));

	}
	return preconditions;
}

MOGoapState* ErectAction::GetEffects(MOGoapActionStackData& stackData)
{
	effects->Set("hasErect", true);
	return effects;
}

bool ErectAction::CheckProceduralCondition(MOGoapActionStackData& stackData)
{


	return true;
}

void ErectAction::UpdateAction(float DeltaTime)
{
	TotalTime += DeltaTime;
	if (TotalTime >= DelayTime)
	{
		DoneCallBack(this);
		TotalTime = 0.0f;
	}
}

float ErectAction::GetCost(MOGoapActionStackData& stackData)
{
	//º‹…Ë ±º‰
	float extraCost = 2.0f;

	return MOGoapAction::GetCost(stackData) + extraCost;
}
