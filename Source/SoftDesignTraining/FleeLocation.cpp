// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "FleeLocation.h"


// Sets default values
AFleeLocation::AFleeLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFleeLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFleeLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

