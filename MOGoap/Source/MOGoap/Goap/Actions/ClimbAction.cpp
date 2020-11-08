#include "ClimbAction.h"
#include "GameFramework/Character.h"
#include "../Core/MOGoapAgent.h"
#include <Engine/Engine.h>
#include "../../MOGoapAI.h"
#include "../../MOGoapManager.h"
#include "../../EntryPoint.h"

ClimbAction::ClimbAction():MOGoapAction()
{
	ActionName = "ClimbAction";

	DelayTime = 2.0f;

	//TargetID = "EntryPoint";
}

std::vector<MOGoapState*> ClimbAction::GetSettings(const MOGoapActionStackData& stackData)
{
	if (SettingsList.size() > 0) {
		return SettingsList;
	}
	ActionSettings->Clear();

	if (AMOGoapManager::EntryPoints.Num() > 0)
	{
		float dis = FLT_MAX;
		float tmp = 0;
		for (auto& entry : AMOGoapManager::EntryPoints)
		{
			if (entry->bOpen && !entry->IsPendingKill())
			{
				tmp = FVector::Dist2D(entry->GetActorLocation(), Agent->GetOwner()->GetActorLocation());
				if (tmp < dis)
				{
					dis = tmp;
					
					ActionSettings->Set("EntryPoint", entry);
					ActionSettings->Set("ClimbTarget", entry->GetPathEnd());
				}
			}
		}
		SettingsList.push_back(ActionSettings->Clone());
	}

	return SettingsList;
}

void ClimbAction::Run(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState, ActionCallBack done /*= nullptr*/, ActionCallBack fail /*= nullptr*/)
{
	MOGoapAction::Run(previousAction, nextAction, settings, goalState, done);

	Agent->GetOwner()->MoveTo(settings->GetValue<FVector>("ClimbTarget"), this);

	if (Agent != nullptr)
	{
		Agent->GetOwner()->PrintAction(ActionName.c_str());
	}
}


MOGoapState* ClimbAction::GetPreconditions(MOGoapActionStackData& stackData)
{
	preconditions->Clear();
	if (stackData.settings->HasKey("ClimbTarget"))
	{
		//auto entry = stackData.settings->GetValue<AActor*>("resource" + TargetID);

		preconditions->Set("isAtPosition", stackData.settings->GetValue<FVector>("ClimbTarget"));
	}
	
	return preconditions;
}

MOGoapState* ClimbAction::GetEffects(MOGoapActionStackData& stackData)
{
	effects->Clear();
	if (stackData.settings->HasKey("EntryPoint"))
	{
		effects->Set("onWall", true);
	}
	return effects;
}

bool ClimbAction::CheckProceduralCondition(MOGoapActionStackData& stackData)
{
	return true;
}

void ClimbAction::UpdateAction(float DeltaTime)
{
	TotalTime += DeltaTime;
	if (TotalTime >= DelayTime)
	{
		DoneCallBack(this);
		TotalTime = 0.0f;
	}
}

float ClimbAction::GetCost(MOGoapActionStackData& stackData)
{
	//≈ ≈¿ ±º‰
	float extraCost = 2.0f;

	return MOGoapAction::GetCost(stackData) + extraCost;
}
