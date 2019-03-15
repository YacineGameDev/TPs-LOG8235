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
	USDTPathFollowingComponent* pathFollowingComponent = (USDTPathFollowingComponent*)GetPathFollowingComponent();
	//path = pathFollowingComponent->GetPath();
	if (bestTarget != nullptr) { //shouldn't be necessary
		UE_LOG(LogTemp, Warning, TEXT("%s is moving to %s"), *GetPawn()->GetName(), *bestTarget->GetName());
		OnMoveToTarget();
		EPathFollowingRequestResult::Type result = MoveToLocation(bestTarget->GetActorLocation(), -1.0f, false, true, false, true, 0, false);
		if (result == 0) //AlreadyAtGoal
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed"));
		}
		else if (result == 1) {
			UE_LOG(LogTemp, Warning, TEXT("AlreadyAtGoal"));

		}
		else if (result == 2) {
			UE_LOG(LogTemp, Warning, TEXT("Success"));
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
	APawn* selfPawn = GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("%s has finish his move"), *selfPawn->GetName());
	bestTarget = nullptr;
    m_ReachedTarget = true;
}

void ASDTAIController::ShowNavigationPath()
{
	//Show current navigation path DrawDebugLine and DrawDebugSphere
	USDTPathFollowingComponent* pathFollowingComponent = (USDTPathFollowingComponent*)GetPathFollowingComponent();
	const auto path = pathFollowingComponent->GetPath();
	if (path.IsValid()) {
		UWorld* word = GetWorld();
		const TArray<FNavPathPoint>& points = path->GetPathPoints();
		TArray<FVector> pointsLocation;
		FVector previousPointLocation = { 0, 0, 0 };
		for (const FNavPathPoint& point : points)
		{
			DrawDebugSphere(word, point.Location, 10, 30, FColor::Purple);
			if (previousPointLocation.IsZero()) //first iteration
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
	DrawDebugCapsule(GetWorld(), detectionStartLocation + m_DetectionCapsuleHalfLength * selfPawn->GetActorForwardVector(), m_DetectionCapsuleHalfLength, m_DetectionCapsuleRadius, selfPawn->GetActorQuat() * selfPawn->GetActorUpVector().ToOrientationQuat(), FColor::Blue);
    //Set behavior based on hit
	/*if (bestTarget != nullptr)
	UE_LOG(LogTemp, Warning, TEXT("%s is the best target"), *bestTarget->GetName());*/

	if (detectionHit.GetActor() != nullptr) 
	{
		if (IsPlayer(detectionHit) || IsCollectibleAndAvailable(detectionHit))
		{
			bestTarget = detectionHit.GetActor();
		}
	}
	else if (bestTarget == nullptr)
	{
		bestTarget = GetNearestColectible();
	}
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
	TArray<AActor*> foundCollectibles;
	UGameplayStatics::GetAllActorsOfClass(word, ASDTCollectible::StaticClass(), foundCollectibles);
	AActor* nearestCollectible = nullptr;
	float nearestDistance = TNumericLimits<float>::Max();
	for (AActor* collectible : foundCollectibles)
	{
		if(nearestDistance > collectible->GetSquaredDistanceTo(selfPawn) && !Cast<ASDTCollectible>(collectible)->IsOnCooldown())
		{
			nearestCollectible = collectible;
			nearestDistance = collectible->GetSquaredDistanceTo(selfPawn);
		}
	}
	return nearestCollectible;
}

bool ASDTAIController::IsCollectibleAndAvailable(FHitResult hit) {
	if (hit.GetComponent()->GetCollisionObjectType() == COLLISION_COLLECTIBLE)
		return !Cast<ASDTCollectible>(hit.GetActor())->IsOnCooldown();
	return false;
}

bool ASDTAIController::IsPlayer(FHitResult hit) {
	return hit.GetComponent()->GetCollisionObjectType() == COLLISION_PLAYER;
}