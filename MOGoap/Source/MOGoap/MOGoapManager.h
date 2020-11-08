// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include "MOGoapManager.generated.h"

class APrimitiveResource;
class AEntryPoint;

UCLASS()
class MOGOAP_API AMOGoapManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMOGoapManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	static TArray<APrimitiveResource*> Resources;
	static TArray<AEntryPoint*> EntryPoints;
};
