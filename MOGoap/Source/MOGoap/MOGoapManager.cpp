// Fill out your copyright notice in the Description page of Project Settings.


#include "MOGoapManager.h"
#include "Goap/Core/MOGoapPlannerManager.h"
#include <Kismet/GameplayStatics.h>
#include "PrimitiveResource.h"
#include "EntryPoint.h"

TArray<APrimitiveResource*> AMOGoapManager::Resources;
TArray<AEntryPoint*> AMOGoapManager::EntryPoints;
// Sets default values
AMOGoapManager::AMOGoapManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMOGoapManager::BeginPlay()
{
	Super::BeginPlay();
	
	MOGoapPlannerManager::GetInstance()->Start();

	Resources.Empty();
	EntryPoints.Empty();

	TArray<AActor*> actorsFound;
	UGameplayStatics::GetAllActorsOfClass(this, APrimitiveResource::StaticClass(), actorsFound);
	
	for (auto& a : actorsFound)
	{
		Resources.Add(Cast<APrimitiveResource>(a));
	}
	actorsFound.Empty();

	UGameplayStatics::GetAllActorsOfClass(this, AEntryPoint::StaticClass(), actorsFound);

	for (auto& a : actorsFound)
	{
		EntryPoints.Add(Cast<AEntryPoint>(a));
	}


}

// Called every frame
void AMOGoapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MOGoapPlannerManager::GetInstance()->Update();

}

