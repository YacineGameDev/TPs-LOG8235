// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_TryIsFleeing.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API UBTService_TryIsFleeing : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_TryIsFleeing();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void OnFleeTimerDone();

protected:

	FTimerHandle fleeTimer;

};
