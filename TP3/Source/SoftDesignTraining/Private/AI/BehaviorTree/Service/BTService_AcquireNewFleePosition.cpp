// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "BTService_AcquireNewFleePosition.h"
#include "SDTFleeLocation.h"
#include "EngineUtils.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "SDTAIController.h"


UBTService_AcquireNewFleePosition::UBTService_AcquireNewFleePosition()
{
	bCreateNodeInstance = true;
}

void UBTService_AcquireNewFleePosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
	{
		if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
		{
			if (aiController->m_ReachedTarget)
			{
				APawn* pawn = Cast<APawn>(MyBlackboard->GetValue<UBlackboardKeyType_Object>(aiController->GetPawnBBKeyID()));
				MyBlackboard->SetValue<UBlackboardKeyType_Vector>(aiController->GetFleePosBBKeyID(), GetBestFleeLocation(pawn));
			}
		}
	}
}

FVector UBTService_AcquireNewFleePosition::GetBestFleeLocation(APawn* pawn)
{
	float bestLocationScore = 0.f;
	ASDTFleeLocation* bestFleeLocation = nullptr;


	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	for (TActorIterator<ASDTFleeLocation> actorIterator(GetWorld(), ASDTFleeLocation::StaticClass()); actorIterator; ++actorIterator)
	{
		ASDTFleeLocation* fleeLocation = Cast<ASDTFleeLocation>(*actorIterator);
		if (fleeLocation)
		{
			float distToFleeLocation = FVector::Dist(fleeLocation->GetActorLocation(), playerCharacter->GetActorLocation());

			FVector selfToPlayer = playerCharacter->GetActorLocation() - pawn->GetActorLocation();
			selfToPlayer.Normalize();

			FVector selfToFleeLocation = fleeLocation->GetActorLocation() - pawn->GetActorLocation();
			selfToFleeLocation.Normalize();

			float fleeLocationToPlayerAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(selfToPlayer, selfToFleeLocation)));
			float locationScore = distToFleeLocation + fleeLocationToPlayerAngle * 100.f;

			if (locationScore > bestLocationScore)
			{
				bestLocationScore = locationScore;
				bestFleeLocation = fleeLocation;
			}
		}
	}

	FVector result = FVector::ZeroVector;
	if (bestFleeLocation) {
		result = bestFleeLocation->GetActorLocation();
	}

	return result;
}

