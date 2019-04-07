// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BTService_TryDetectTarget.h"


UBTService_TryDetectTarget::UBTService_TryDetectTarget()
{
	bCreateNodeInstance = true;
}

void UBTService_TryDetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
	{
		//Trigger from service the detect
		aiController->DetectPlayer();
		if (aiController->IsTargetPlayerSeen())
		{
			//write to bb that the player is seen
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetSeenKeyID(), true);
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetPoweredUpKeyID(), aiController->IsPlayerPoweredUp());
			if (aiController->IsPlayerPoweredUp()) {
				//write to bb the flee position
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetFleePosBBKeyID(), aiController->GetFleePos());
			}
			else {
				//write to bb the position of the target
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetTargetPosBBKeyID(), aiController->GetTargetPlayerPos());
			}
		}
		else
		{
			//write to bb that the player is not seen
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetSeenKeyID(), false);
			// Collectible Pos
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetRandomCollectiblePosBBKeyID(), aiController->GetRandomCollectiblePos());
		}
	}
}



