// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "SDTCollectible.h"
#include "SDTFleeLocation.h"
#include "SDTPathFollowingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "UnrealMathUtility.h"
#include "SDTUtils.h"
#include "EngineUtils.h"

ASDTAIController::ASDTAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USDTPathFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void ASDTAIController::GoToBestTarget(float deltaTime)
{
	if (bestTarget != nullptr) {
		OnMoveToTarget();
		APawn* selfPawn = GetPawn();
		UE_LOG(LogTemp, Warning, TEXT("%s is moving to : %s"), *selfPawn->GetName(), *bestTarget->GetName());
		MoveToActor(bestTarget);
		USDTPathFollowingComponent* pathFollowingComponent = (USDTPathFollowingComponent*)GetPathFollowingComponent();
		const auto path = pathFollowingComponent->GetPath();
		if (path.IsValid()) {
			pathFollowingComponent->FollowPathSegment(deltaTime);
		}

	}
}

void ASDTAIController::OnMoveToTarget()
{
    m_ReachedTarget = false;
}

void ASDTAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    m_ReachedTarget = true;
}

void ASDTAIController::ShowNavigationPath()
{
	//Show current navigation path DrawDebugLine and DrawDebugSphere
	USDTPathFollowingComponent* pathFollowingComponent = (USDTPathFollowingComponent*)GetPathFollowingComponent();
	const auto path = pathFollowingComponent->GetPath();
	UWorld* word = GetWorld();
	const TArray<FNavPathPoint>& points = path->GetPathPoints();
	TArray<FVector> pointsLocation;
	FVector previousPointLocation = {0, 0, 0};
	for (const FNavPathPoint& point : points) 
	{
		DrawDebugSphere(word, point.Location, 10, 30, FColor::Purple);
		if (previousPointLocation.IsZero()) 
		{
			previousPointLocation = point.Location;
		}
		else
		{
			DrawDebugLine(word, previousPointLocation, point.Location, FColor::Red);
			previousPointLocation = point.Location;
		}
	}
}

void ASDTAIController::ChooseBehavior(float deltaTime)
{
    UpdatePlayerInteraction(deltaTime);
}

void ASDTAIController::UpdatePlayerInteraction(float deltaTime)
{
    //finish jump before updating AI state
    if (AtJumpSegment)
        return;

    APawn* selfPawn = GetPawn();
    if (!selfPawn)
        return;

    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!playerCharacter)
        return;

    FVector detectionStartLocation = selfPawn->GetActorLocation() + selfPawn->GetActorForwardVector() * m_DetectionCapsuleForwardStartingOffset;
    FVector detectionEndLocation = detectionStartLocation + selfPawn->GetActorForwardVector() * m_DetectionCapsuleHalfLength * 2;

    TArray<TEnumAsByte<EObjectTypeQuery>> detectionTraceObjectTypes;
    detectionTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_COLLECTIBLE));
    detectionTraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_PLAYER));

    TArray<FHitResult> allDetectionHits;
    GetWorld()->SweepMultiByObjectType(allDetectionHits, detectionStartLocation, detectionEndLocation, FQuat::Identity, detectionTraceObjectTypes, FCollisionShape::MakeSphere(m_DetectionCapsuleRadius));

    FHitResult detectionHit;
    GetHightestPriorityDetectionHit(allDetectionHits, detectionHit);

    //Set behavior based on hit
	
	if (detectionHit.GetActor() == nullptr && bestTarget == nullptr) {
		bestTarget = GetNearestColectible();
	}

	if (detectionHit.GetActor() != bestTarget && detectionHit.GetActor() !=  nullptr)  // new best target
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s is seing something new : %s"), *selfPawn->GetName(), *detectionHit.GetActor()->GetName());
		bestTarget = detectionHit.GetActor();
	}
    DrawDebugCapsule(GetWorld(), detectionStartLocation + m_DetectionCapsuleHalfLength * selfPawn->GetActorForwardVector(), m_DetectionCapsuleHalfLength, m_DetectionCapsuleRadius, selfPawn->GetActorQuat() * selfPawn->GetActorUpVector().ToOrientationQuat(), FColor::Blue);
}

void ASDTAIController::GetHightestPriorityDetectionHit(const TArray<FHitResult>& hits, FHitResult& outDetectionHit)
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

void ASDTAIController::AIStateInterrupted()
{
    StopMovement();
    m_ReachedTarget = true;
}

AActor* ASDTAIController::GetNearestColectible() {
	UWorld* word = GetWorld();
	APawn* selfPawn = GetPawn();
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(word, ASDTCollectible::StaticClass(), foundActors);
	AActor* nearestCollectible = *foundActors.GetData();

	for (AActor* collectible : foundActors) 
	{
		if(nearestCollectible->GetSquaredDistanceTo(selfPawn) > collectible->GetSquaredDistanceTo(selfPawn))
		{
			nearestCollectible = collectible;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s is going to : %s"), *selfPawn->GetName(), *nearestCollectible->GetName());
	return nearestCollectible;
}