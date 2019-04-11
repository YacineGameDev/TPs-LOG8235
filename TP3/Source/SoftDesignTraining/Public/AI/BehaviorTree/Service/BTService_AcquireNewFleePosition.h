// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AcquireNewFleePosition.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API UBTService_AcquireNewFleePosition : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_AcquireNewFleePosition();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	FVector GetBestFleeLocation(APawn* pawn);

	
};
