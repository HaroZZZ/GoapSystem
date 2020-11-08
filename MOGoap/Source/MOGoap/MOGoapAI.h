// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MOGoapAI.generated.h"

class MOGoapAction;

UCLASS()
class MOGOAP_API AMOGoapAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMOGoapAI();

	class MOGoapAgent* GoapAgent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Destroyed() override;
	UFUNCTION(BlueprintImplementableEvent)
	void PrintAction(const FString& name);

	UFUNCTION(BlueprintImplementableEvent)
	void PrintGoal(const FString& name);

	UFUNCTION(BlueprintImplementableEvent)
	void PrintActionNum(const int32& num);

	UFUNCTION(BlueprintImplementableEvent)
	void PrintPosition(FVector pos);

	bool MoveTo(FVector pos,MOGoapAction* action);

	MOGoapAgent* GetAgent()
	{
		return GoapAgent;
	}

	MOGoapAction* CurrentAcion;
};
