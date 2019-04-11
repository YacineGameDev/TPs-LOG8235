// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_TryDetectTarget.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API UBTService_TryDetectTarget : public UBTService
{
	GENERATED_BODY()

public:

	UBTService_TryDetectTarget();
    
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	bool IsPlayerSeen(const TArray<FHitResult>& hits);
	bool HasLoSOnHit(const FHitResult& hit);

};
