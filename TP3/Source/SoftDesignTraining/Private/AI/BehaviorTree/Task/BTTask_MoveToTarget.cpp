// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"

#include "SDTAIController.h"
#include "SoftDesignTrainingCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "AI/Navigation/NavigationSystem.h"

#include "BTTask_MoveToTarget.h"



EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
	{
		if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
		{

			if (!aiController->AtJumpSegment)
			{
				FVector targetPosition = FVector::ZeroVector;
				FVector currentTargetLocation = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

				aiController->MoveToLocation(currentTargetLocation, 0.5f, false, true, false, NULL, false);
				aiController->OnMoveToTarget();
			}
			
			/*switch (aiController->GetPlayerInteractionBehavior())
			{
			case aiController->PlayerInteractionBehavior_Chase:
				aiController->MoveToPlayer();
				break;
			case aiController->PlayerInteractionBehavior_Flee:
				aiController->MoveToLocation(currentTargetLocation, 0.5f, false, true, false, NULL, false);
				aiController->OnMoveToTarget();
				break;
			case aiController->PlayerInteractionBehavior_Collect:
				aiController->MoveToLocation(currentTargetLocation, 0.5f, false, true, false, NULL, false);
				aiController->OnMoveToTarget();

				break;
			}*/

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
