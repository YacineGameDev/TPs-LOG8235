// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BTService_TryDetectTarget.h"
#include "SDTAIController.h"
#include "SDTUtils.h"



UBTService_TryDetectTarget::UBTService_TryDetectTarget()
{
	bCreateNodeInstance = true;
}

void UBTService_TryDetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent()) 
	{
		if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
		{

			//Trigger from service the detect
			// aiController->DetectPlayer();m_ReachedTargetMTrue
			//aiController->UpdatePlayerInteraction(DeltaSeconds);
			//UE_LOG(LogTemp, Warning, TEXT("m_ReachedTarget is %s"), (aiController->m_ReachedTarget ? TEXT("True"): TEXT("False")));

			//finish jump before updating AI state
			/*if (AtJumpSegment)
				return;*/


			/*APawn* selfPawn = Cast<APawn>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(aiController->GetPawnBBKeyID()));
			if (!selfPawn)
				return;*/

			/*ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (!playerCharacter)
			return;*/

			/*FVector detectionStartLocation = selfPawn->GetActorLocation() + selfPawn->GetActorForwardVector() * aiController->m_DetectionCapsuleForwardStartingOffset;
			FVector detectionEndLocation = detectionStartLocation + selfPawn->GetActorForwardVector() * aiController->m_DetectionCapsuleHalfLength * 2;

			TArray<TEnumAsByte<EObjectTypeQuery>> detectionTraceObjectTypes;
			detectionTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_PLAYER));

			TArray<FHitResult> allDetectionHits;
			GetWorld()->SweepMultiByObjectType(allDetectionHits, detectionStartLocation, detectionEndLocation, FQuat::Identity, detectionTraceObjectTypes, FCollisionShape::MakeSphere(aiController->m_DetectionCapsuleRadius));

			FHitResult detectionHit;
			GetHightestPriorityDetectionHit(allDetectionHits, detectionHit);

			UpdatePlayerInteractionBehavior(detectionHit, deltaTime);

			if (GetMoveStatus() == EPathFollowingStatus::Idle)
			{
				m_ReachedTarget = true;
			}




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

			}*/
		}
	}
}



void UBTService_TryDetectTarget::GetHightestPriorityDetectionHit(const TArray<FHitResult>& hits, FHitResult& outDetectionHit)
{
	for (const FHitResult& hit : hits)
	{
		if (UPrimitiveComponent* component = hit.GetComponent())
		{
			if (component->GetCollisionObjectType() == COLLISION_PLAYER)
			{
				//we can't get more important than the player
				outDetectionHit = hit;
				return;
			}
			else if (component->GetCollisionObjectType() == COLLISION_COLLECTIBLE)
			{
				outDetectionHit = hit;
			}
		}
	}
}
