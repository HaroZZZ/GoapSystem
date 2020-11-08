#include "AttackAction.h"
#include "GameFramework/Character.h"
#include "../Core/MOGoapAgent.h"
#include <Engine/Engine.h>
#include "../../MOGoapAI.h"
#include "../../PrimitiveResource.h"
#include "../../MOGoapManager.h"

AttackAction::AttackAction():MOGoapAction()
{
	ActionName = "AttackAction";

	//effects->Set("canMove", true);

	//ActionSettings->Set("GotoPoint", true);

	DelayTime = 1.0f;

	//TargetID = "enemy";
}

std::vector<MOGoapState*> AttackAction::GetSettings(const MOGoapActionStackData& stackData)
{
	ActionSettings->Clear();
	SettingsList.clear();

	if (AMOGoapManager::Resources.Num() > 0)
	{
		for (auto& resource : AMOGoapManager::Resources)
		{
			float dis = FLT_MAX;
			float tmp = 0;
			if (resource->GetResourceName() == "enemy" && !resource->IsPendingKill())
			{
				tmp = FVector::Dist2D(resource->GetActorLocation(), Agent->GetOwner()->GetActorLocation());
				if (tmp < dis)
				{
					dis = tmp;
					ActionSettings->Set("enemyPosition", resource->GetActorLocation());
					ActionSettings->Set("enemy", resource);
				}
			}
		}
		SettingsList.push_back(ActionSettings->Clone());
	}

	return SettingsList;
}

void AttackAction::Run(MOGoapAction* previousAction, MOGoapAction* nextAction, MOGoapState* settings, MOGoapState* goalState, ActionCallBack done /*= nullptr*/, ActionCallBack fail /*= nullptr*/)
{
	MOGoapAction::Run(previousAction, nextAction, settings, goalState, done);
	if (settings->HasKey("enemy"))
	{
		settings->GetValue<AActor*>("enemy")->Destroy();
		DoneCallBack(this);
	}

	if (Agent != nullptr)
	{
		Agent->GetOwner()->PrintAction(ActionName.c_str());
	}
}

MOGoapState* AttackAction::GetPreconditions(MOGoapActionStackData& stackData)
{
	preconditions->Clear();

	if (stackData.settings->HasKey("enemy"))
	{
		auto pos = stackData.settings->GetValue<FVector>("enemyPosition");

		preconditions->Set("isAtPosition", pos);
		preconditions->Set("hasPower", true);
		if (pos.Z > Agent->GetOwner()->GetActorLocation().Z)
		{
			preconditions->Set("onWall", true);
		}
	}

	return preconditions;
}

MOGoapState* AttackAction::GetEffects(MOGoapActionStackData& stackData)
{
	if (stackData.settings->HasKey("enemy"))
	{
		effects->Set("enemyDie", true);
	}
	else
	{
		effects->Clear();
	}

	return effects;
}

bool AttackAction::CheckProceduralCondition(MOGoapActionStackData& stackData)
{
	return true;
}

void AttackAction::UpdateAction(float DeltaTime)
{

}
