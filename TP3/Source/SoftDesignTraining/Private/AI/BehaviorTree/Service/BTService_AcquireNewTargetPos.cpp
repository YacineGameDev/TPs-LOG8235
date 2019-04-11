// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "BTService_AcquireNewTargetPos.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "SDTAIController.h"


UBTService_AcquireNewTargetPos::UBTService_AcquireNewTargetPos()
{
	bCreateNodeInstance = true;
}

void UBTService_AcquireNewTargetPos::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
	{
		ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetTargetPosBBKeyID(), playerCharacter->GetActorLocation());
	}
}