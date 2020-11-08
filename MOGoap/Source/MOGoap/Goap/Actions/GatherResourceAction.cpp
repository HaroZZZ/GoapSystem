#include "GatherResourceAction.h"
#include "../../MOGoapAI.h"
#include "../Core/MOGoapAgent.h"
#include <Engine/Engine.h>
#include "../../PrimitiveResource.h"
#include "../../MOGoapManager.h"

GatherResourceAction::GatherResourceAction():MOGoapAction()
{
	ActionName = "GatherResourceAction";

	//ActionSettings->Set("GotoPoint", true);

	DelayTime = 1.0f;

	GatherRes = false;
}

std::vector<MOGoapState*> GatherResourceAction::GetSettings(const MOGoapActionStackData& stackData)
{	
	ActionSettings->Clear();
	SettingsList.clear();
	GatherRes = false;
	std::string TargetID;

	for (auto& pair : stackData.goalState->values)
	{
		if (pair.first.substr(0, 3) == "has")
		{
			TargetID = pair.first.substr(3);

			break;
		}
	}

	if (AMOGoapManager::Resources.Num() > 0)
	{
		for (auto& resource : AMOGoapManager::Resources)
		{
			float dis = FLT_MAX;
			float tmp = 0;
			if (resource->ResourceID == TargetID.c_str() && !resource->IsPendingKill())
			{
				tmp = FVector::Dist2D(resource->GetActorLocation(), Agent->GetOwner()->GetActorLocation());
				if (tmp < dis)
				{
					dis = tmp;
					ActionSettings->Set("resourcePosition", resource->GetActorLocation());
					ActionSettings->Set("resource", resource);
				}
			}
		}
		SettingsList.push_back(ActionSettings->Clone());
	}

	return SettingsList;
}

void GatherResourceAction::Run(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState, ActionCallBack done /*= nullptr*/, ActionCallBack fail /*= nullptr*/)
{
	MOGoapAction::Run(previousAction, nextAction, settings, goalState, done);

	if (settings->HasKey("resource"))
	{
		GatherRes = settings->GetValue<AActor*>("resource")->Destroy();
	}

	if (Agent != nullptr)
	{
		Agent->GetOwner()->PrintAction(ActionName.c_str());
	}
}

MOGoapState* GatherResourceAction::GetPreconditions(MOGoapActionStackData& stackData)
{
	preconditions->Clear();
	if (stackData.settings->HasKey("resourcePosition"))
	{
		preconditions->Set("isAtPosition", stackData.settings->Get("resourcePosition"));
	}

	return preconditions;
}

MOGoapState* GatherResourceAction::GetEffects(MOGoapActionStackData& stackData)
{
	effects->Clear();
	if (stackData.settings->HasKey("resource"))
	{
		std::string id = Cast<APrimitiveResource>(stackData.settings->GetValue<AActor*>("resource"))->GetResourceName();
		effects->Set("has" + id, true);
	}

	return effects;
}

bool GatherResourceAction::CheckProceduralCondition(MOGoapActionStackData& stackData)
{
	if (stackData.goalState->HasKey("onWall"))
	{
		return false;
	}
	return true;
}

void GatherResourceAction::UpdateAction(float DeltaTime)
{
	TotalTime += DeltaTime;
	if (TotalTime >= DelayTime && GatherRes == true)
	{
		DoneCallBack(this);
		TotalTime = 0.0f;
	}
}

float GatherResourceAction::GetCost(MOGoapActionStackData& stackData)
{
	float extraCost = 0.0f;

	if (stackData.settings->HasKey("resourcePosition") && stackData.currentState->HasKey("isAtPosition"))
	{
		extraCost += FVector::Distance(stackData.settings->GetValue<FVector>("resourcePosition"), stackData.currentState->GetValue<FVector>("isAtPosition")) / 500.0f;
	}

	return MOGoapAction::GetCost(stackData) + extraCost;
}
