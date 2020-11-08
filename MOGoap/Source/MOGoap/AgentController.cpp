// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentController.h"
#include "MOGoapAI.h"
#include "Goap/Core/MOGoapAction.h"
#include "Goap/Core/MOGoapAgent.h"

void AAgentController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	if(Cast<AMOGoapAI>(GetPawn())->CurrentAcion != nullptr && Result.Flags == FPathFollowingResultFlags::Success)
		Cast<AMOGoapAI>(GetPawn())->CurrentAcion->ActionDone();
}
