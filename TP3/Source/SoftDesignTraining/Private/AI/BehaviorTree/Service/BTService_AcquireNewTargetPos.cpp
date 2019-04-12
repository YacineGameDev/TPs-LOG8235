// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "BTService_AcquireNewTargetPos.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "SDTAIController.h"
#include "DrawDebugHelpers.h"


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



			// if distance relative a player > Seuil
				// targetPPositon = groupManager -> assigne position;
				// OwnerComp.GetBlackboardComponent()->SetValue() <- setter la valeur de targetPos dans le BB a targetPPosition	
			// sinon
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetTargetPosBBKeyID(), playerCharacter->GetActorLocation());
			
			DrawDebugSphere(GetWorld(), selfPawn->GetActorLocation() + FVector(0.f, 0.f, 100.f), 40.0f, 32, FColor::Purple);
		}
	}
}