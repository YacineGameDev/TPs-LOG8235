// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BTService_TryDetectTarget.h"
#include "SDTAIController.h"



UBTService_TryDetectTarget::UBTService_TryDetectTarget()
{
	bCreateNodeInstance = true;
}

void UBTService_TryDetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
	{

		//Trigger from service the detect
		// aiController->DetectPlayer();m_ReachedTargetMTrue
		//aiController->UpdatePlayerInteraction(DeltaSeconds);
		//UE_LOG(LogTemp, Warning, TEXT("m_ReachedTarget is %s"), (aiController->m_ReachedTarget ? TEXT("True"): TEXT("False")));

		if (aiController->m_ReachedTarget) {
			UE_LOG(LogTemp, Warning, TEXT("On entre"));
			switch (aiController->GetPlayerInteractionBehavior())
			{
			case aiController->PlayerInteractionBehavior_Chase:
				//UE_LOG(LogTemp, Warning, TEXT("Chase"));
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetSeenKeyID(), true);
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetPoweredUpKeyID(), false);
				break;
			case aiController->PlayerInteractionBehavior_Flee:
				//UE_LOG(LogTemp, Warning, TEXT("Flee"));
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetSeenKeyID(), true);
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetPoweredUpKeyID(), true);
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetFleePosBBKeyID(), aiController->GetBestFleeLocation());

				break;
			case aiController->PlayerInteractionBehavior_Collect:
				//UE_LOG(LogTemp, Warning, TEXT("Collect"));
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetSeenKeyID(), false);
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetRandomCollectiblePosBBKeyID(), aiController->GetRandomCollectibleLocation());
				break;
			}

		}
	}
}



