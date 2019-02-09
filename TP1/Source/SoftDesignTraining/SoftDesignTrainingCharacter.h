// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Engine.h"
#include "SoftDesignTrainingCharacter.generated.h"

UCLASS()
class ASoftDesignTrainingCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASoftDesignTrainingCharacter();

    virtual void BeginPlay() override;
	virtual void OnCollectPowerUp() { score++; };
	virtual void IncrementScore();
	virtual void IncrementDeathToll();
	int GetScore() { return score; }
	int GetDeathToll() { return death_toll; }
	
	
protected:
    UFUNCTION()
    virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    FVector m_StartingPosition;

	int score = 0;
	int death_toll = 0;
};

