// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AcquireNewPickUpPos.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API UBTService_AcquireNewPickUpPos : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_AcquireNewPickUpPos();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	FVector GetRandomCollectibleLocation();
	
};
