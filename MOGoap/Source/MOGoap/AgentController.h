// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AgentController.generated.h"

/**
 * 
 */
UCLASS()
class MOGOAP_API AAgentController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);
};
