// Fill out your copyright notice in the Description page of Project Settings.


#include "MOGoapAI.h"
#include "Goap/Core/MOGoapAgent.h"
#include <Engine/Engine.h>
#include <AIController.h>
#include "AgentController.h"
#include "Goap/Core/MOGoapAction.h"
// Sets default values
AMOGoapAI::AMOGoapAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GoapAgent = new MOGoapAgent(this);

	AIControllerClass = AAgentController::StaticClass();
}

// Called when the game starts or when spawned
void AMOGoapAI::BeginPlay()
{
	Super::BeginPlay();
	GoapAgent->Start();
}

// Called every frame
void AMOGoapAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GoapAgent->Update(DeltaTime);
}

// Called to bind functionality to input
void AMOGoapAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AMOGoapAI::Destroyed()
{
	delete GoapAgent;
	GoapAgent = nullptr;

}

bool AMOGoapAI::MoveTo(FVector pos, MOGoapAction* action)
{
	Cast<AAgentController>(GetController())->MoveToLocation(pos);
	CurrentAcion = action;
	return true;
}
