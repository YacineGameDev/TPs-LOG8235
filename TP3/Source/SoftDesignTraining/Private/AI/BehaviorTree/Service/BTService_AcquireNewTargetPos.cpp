// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "BTService_AcquireNewTargetPos.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "SDTAIController.h"
#include "DrawDebugHelpers.h"
#include "AiAgentGroupManager.h"


UBTService_AcquireNewTargetPos::UBTService_AcquireNewTargetPos()
{
	bCreateNodeInstance = true;
}

void UBTService_AcquireNewTargetPos::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
	{
		if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
		{
			
			APawn* selfPawn = Cast<APawn>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(aiController->GetPawnBBKeyID()));
			if (!selfPawn)
				return;

			ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (!playerCharacter)
				return;

			AiAgentGroupManager* groupManager = AiAgentGroupManager::GetInstance();

			FVector targetLocation = FVector::ZeroVector;
			float distance = (playerCharacter->GetActorLocation() - selfPawn->GetActorLocation()).Size();
			if (distance > 150.f)
			{
				targetLocation = groupManager->allocateTargetPos(Cast<ACharacter>(selfPawn), aiController->targetLocationIdx);
			}
			else
			{
				groupManager->FreeLocation(aiController->targetLocationIdx);
				aiController->targetLocationIdx = -1;
				targetLocation = playerCharacter->GetActorLocation();
			}
			
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetTargetPosBBKeyID(), targetLocation);

			DrawDebugSphere(GetWorld(), selfPawn->GetActorLocation() + FVector(0.f, 0.f, 100.f), 40.0f, 32, FColor::Purple);
		}
	}
}