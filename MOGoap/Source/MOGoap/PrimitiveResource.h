// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <string>
#include "PrimitiveResource.generated.h"

UCLASS()
class MOGOAP_API APrimitiveResource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APrimitiveResource();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ResourceID;

	std::string GetResourceName();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
