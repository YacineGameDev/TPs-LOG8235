// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SoftDesignTrainingCharacter.generated.h"


UCLASS()
class ASoftDesignTrainingCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASoftDesignTrainingCharacter();

    virtual void BeginPlay() override;
    virtual void OnCollectPowerUp() {};
    void Die();

	UBehaviorTree* GetBehaviorTree() const { return m_aiBehaviorTree; }

protected:
    UFUNCTION()
    virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    FVector m_StartingPosition;

	UPROPERTY(EditAnywhere, category = Behavior)
	UBehaviorTree* m_aiBehaviorTree;

};

