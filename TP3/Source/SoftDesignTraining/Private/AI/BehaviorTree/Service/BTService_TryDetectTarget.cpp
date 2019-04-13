// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BTService_TryDetectTarget.h"
#include "SDTAIController.h"
#include "SDTUtils.h"
#include "AiAgentGroupManager.h"
#include "FpsManager.h"


UBTService_TryDetectTarget::UBTService_TryDetectTarget()
{
	bCreateNodeInstance = true;
}

void UBTService_TryDetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FpsManager* FPSmanager = FpsManager::GetInstance();
	if (!FPSmanager->canExecute()) return;

	float startTime = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());

	if (const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent()) 
	{
		if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
		{
			//finish jump before updating AI state
			if (aiController->AtJumpSegment)
				return;

			APawn* selfPawn = Cast<APawn>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(aiController->GetPawnBBKeyID()));
			if (!selfPawn)
				return;

			ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (!playerCharacter)
				return;

			AiAgentGroupManager* groupeManager = AiAgentGroupManager::GetInstance();

			FVector detectionStartLocation = selfPawn->GetActorLocation() + selfPawn->GetActorForwardVector() * aiController->m_DetectionCapsuleForwardStartingOffset;
			FVector detectionEndLocation = detectionStartLocation + selfPawn->GetActorForwardVector() * aiController->m_DetectionCapsuleHalfLength * 2;

			TArray<TEnumAsByte<EObjectTypeQuery>> detectionTraceObjectTypes;
			detectionTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_PLAYER));

			TArray<FHitResult> allDetectionHits;
			GetWorld()->SweepMultiByObjectType(allDetectionHits, detectionStartLocation, detectionEndLocation, FQuat::Identity, detectionTraceObjectTypes, FCollisionShape::MakeSphere(aiController->m_DetectionCapsuleRadius));

			if (IsPlayerSeen(allDetectionHits))
			{
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetSeenKeyID(), true);
				if (SDTUtils::IsPlayerPoweredUp(GetWorld()))
					OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetPoweredUpKeyID(), true);
				else {
					groupeManager->RegisterAIAgent(selfPawn);
					OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetPoweredUpKeyID(), false);
				}
			}
			else {
				groupeManager->UnregisterAIAgent(selfPawn);
				//UE_LOG(LogTemp, Warning, TEXT("TargetNotSeen"));
				OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Bool>(aiController->GetTargetSeenKeyID(), false);
			}
			aiController->execTime_detectPlayer = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles()) - startTime;
		}
	}
}



bool UBTService_TryDetectTarget::IsPlayerSeen(const TArray<FHitResult>& hits)
{
	for (const FHitResult& hit : hits)
	{
		if (UPrimitiveComponent* component = hit.GetComponent())
		{
			if (component->GetCollisionObjectType() == COLLISION_PLAYER)
			{
				//UE_LOG(LogTemp, Warning, TEXT("JE TE VOIS !!!!!!!!!!!!!"));
				if (HasLoSOnHit(hit))
					return true;
			}
		}
	}
	return false;
}


bool UBTService_TryDetectTarget::HasLoSOnHit(const FHitResult& hit)
{
    if (!hit.GetComponent())
        return false;

    TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
    TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

    FVector hitDirection = hit.ImpactPoint - hit.TraceStart;
    hitDirection.Normalize();

    FHitResult losHit;
    FCollisionQueryParams queryParams = FCollisionQueryParams();
    queryParams.AddIgnoredActor(hit.GetActor());

    GetWorld()->LineTraceSingleByObjectType(losHit, hit.TraceStart, hit.ImpactPoint + hitDirection, TraceObjectTypes, queryParams);

    return losHit.GetActor() == nullptr;
}