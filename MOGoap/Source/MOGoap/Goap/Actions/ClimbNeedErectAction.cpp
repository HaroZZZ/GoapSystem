#include "ClimbNeedErectAction.h"
#include "GameFramework/Character.h"
#include "../Core/MOGoapAgent.h"
#include <Engine/Engine.h>
#include "../../MOGoapAI.h"
#include "../../MOGoapManager.h"
#include "../../EntryPoint.h"

ClimbNeedErectAction::ClimbNeedErectAction():MOGoapAction()
{
	ActionName = "ClimbNeedErectAction";

	DelayTime = 5.0f;

	//TargetID = "EntryPoint";
}

std::vector<MOGoapState*> ClimbNeedErectAction::GetSettings(const MOGoapActionStackData& stackData)
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
			if (!entry->bOpen && !entry->IsPendingKill())
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

void ClimbNeedErectAction::Run(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState, ActionCallBack done /*= nullptr*/, ActionCallBack fail /*= nullptr*/)
{
	MOGoapAction::Run(previousAction, nextAction, settings, goalState, done);

	if (Agent != nullptr)
	{
		Agent->GetOwner()->PrintAction(ActionName.c_str());
	}
}


MOGoapState* ClimbNeedErectAction::GetPreconditions(MOGoapActionStackData& stackData)
{
	preconditions->Clear();
	if (stackData.settings->HasKey("ClimbTarget"))
	{
		//auto entry = stackData.settings->GetValue<AActor*>("resource" + TargetID);

		preconditions->Set("isAtPosition", stackData.settings->GetValue<FVector>("ClimbTarget"));
		preconditions->Set("hasErect", true);
	}
	return preconditions;
}

MOGoapState* ClimbNeedErectAction::GetEffects(MOGoapActionStackData& stackData)
{
	effects->Clear();
	if (stackData.settings->HasKey("EntryPoint"))
	{
		effects->Set("onWall", true);
	}
	return effects;
}

bool ClimbNeedErectAction::CheckProceduralCondition(MOGoapActionStackData& stackData)
{
	return true;
}

void ClimbNeedErectAction::UpdateAction(float DeltaTime)
{
	TotalTime += DeltaTime;
	if (TotalTime >= DelayTime)
	{
		DoneCallBack(this);
		TotalTime = 0.0f;
	}
}

float ClimbNeedErectAction::GetCost(MOGoapActionStackData& stackData)
{
	//≈ ≈¿ ±º‰
	float extraCost = 3.0f;

	return MOGoapAction::GetCost(stackData) + extraCost;
}
