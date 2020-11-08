// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntryPoint.generated.h"

UCLASS()
class MOGOAP_API AEntryPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEntryPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float Score;

	//通道是否开启
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	bool bOpen;

	FVector StartPos;
	FVector EndPos;

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetPathStart();

	UFUNCTION(BlueprintImplementableEvent)
	FVector GetPathEnd();
};
