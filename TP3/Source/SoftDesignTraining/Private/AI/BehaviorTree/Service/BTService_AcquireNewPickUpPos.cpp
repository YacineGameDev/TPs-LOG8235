// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "BTService_AcquireNewPickUpPos.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "SDTAIController.h"
#include "SDTCollectible.h"


UBTService_AcquireNewPickUpPos::UBTService_AcquireNewPickUpPos()
{
	bCreateNodeInstance = true;
}

void UBTService_AcquireNewPickUpPos::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (ASDTAIController* aiController = Cast<ASDTAIController>(OwnerComp.GetAIOwner()))
	{
		if (aiController->m_ReachedTarget)
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(aiController->GetRandomCollectiblePosBBKeyID(), UBTService_AcquireNewPickUpPos::GetRandomCollectibleLocation());
	}
}

FVector UBTService_AcquireNewPickUpPos::GetRandomCollectibleLocation()
{
	TArray<AActor*> foundCollectibles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTCollectible::StaticClass(), foundCollectibles);

	FVector CollectibleLocation = FVector::ZeroVector;


	while (foundCollectibles.Num() != 0)
	{
		int index = FMath::RandRange(0, foundCollectibles.Num() - 1);

		ASDTCollectible* collectibleActor = Cast<ASDTCollectible>(foundCollectibles[index]);

		if (collectibleActor) {
			if (!collectibleActor->IsOnCooldown())
			{
				CollectibleLocation = foundCollectibles[index]->GetActorLocation();
				return CollectibleLocation;
			}
			else
			{
				foundCollectibles.RemoveAt(index);
			}
		}
	}
	return CollectibleLocation;
}


