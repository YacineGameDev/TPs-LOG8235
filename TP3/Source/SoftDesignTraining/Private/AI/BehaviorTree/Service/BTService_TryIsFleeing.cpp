// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "BTService_TryIsFleeing.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "SDTAIController.h"
#include "SDTUtils.h"
#include "AiAgentGroupManager.h"


UBTService_TryIsFleeing::UBTService_TryIsFleeing()
{
	bCreateNodeInstance = true;
}

void UBTService_TryIsFleeing::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
	{
		if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
		{
			bool playerDetected = MyBlackboard->GetValue<UBlackboardKeyType_Bool>(aiController->GetTargetSeenKeyID());
			bool playerPoweredUp = MyBlackboard->GetValue<UBlackboardKeyType_Bool>(aiController->GetTargetPoweredUpKeyID());

			if (playerDetected && playerPoweredUp)
			{
				aiController->AIStateInterrupted();

				APawn* selfPawn = Cast<APawn>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(aiController->GetPawnBBKeyID()));
				if (!selfPawn)
					return;

				AiAgentGroupManager* groupManager = AiAgentGroupManager::GetInstance();
				groupManager->UnregisterAIAgent(selfPawn);
				MyBlackboard->SetValue<UBlackboardKeyType_Bool>(aiController->GetIsFleeingKeyID(), true);
				if (!GetWorld()->GetTimerManager().IsTimerActive(fleeTimer))
				{
					GetWorld()->GetTimerManager().SetTimer(fleeTimer, this, &UBTService_TryIsFleeing::OnFleeTimerDone, 3.f, false);
				}
			}
			else if (!GetWorld()->GetTimerManager().IsTimerActive(fleeTimer))
			{
				MyBlackboard->SetValue<UBlackboardKeyType_Bool>(aiController->GetIsFleeingKeyID(), false);
			}
		}
	}
}

void UBTService_TryIsFleeing::OnFleeTimerDone()
{
	GetWorld()->GetTimerManager().ClearTimer(fleeTimer);
	fleeTimer.Invalidate();
}