// Fill out your copyright notice in the Description page of Project Settings.


#include "PrimitiveResource.h"

// Sets default values
APrimitiveResource::APrimitiveResource()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

std::string APrimitiveResource::GetResourceName()
{
	return std::string(TCHAR_TO_UTF8(*ResourceID));
}

// Called when the game starts or when spawned
void APrimitiveResource::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APrimitiveResource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

