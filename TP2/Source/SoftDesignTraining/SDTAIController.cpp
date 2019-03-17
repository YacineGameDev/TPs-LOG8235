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
	if (bestTarget != nullptr) { //shouldn't be necessary
		OnMoveToTarget();
		if(IsActorPlayer(bestTarget))
			MoveToActor(bestTarget, 0.5f, false, true, true, 0, false);
		else if (lastPlayerPosition != FVector::ZeroVector)
			MoveToLocation(lastPlayerPosition, -1.0f, false, true, false, true, 0, false);
		else {
			MoveToLocation(bestTarget->GetActorLocation(), -1.0f, false, true, false, true, 0, false);
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
	if (Result.IsSuccess())
		lastPlayerPosition = FVector::ZeroVector;
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
	bool canSeeHit = true;
	if (IsActorPlayer(detectionHit.GetActor()))
		canSeeHit = !SDTUtils::Raycast(GetWorld(), selfPawn->GetPawnViewLocation(), detectionHit.GetActor()->GetActorLocation());
	
	DrawDebugCapsule(GetWorld(), detectionStartLocation + m_DetectionCapsuleHalfLength * selfPawn->GetActorForwardVector(), m_DetectionCapsuleHalfLength, m_DetectionCapsuleRadius, selfPawn->GetActorQuat() * selfPawn->GetActorUpVector().ToOrientationQuat(), FColor::Blue);
	ResetIfTooMuchCollectibleTargeted();
    //Set behavior based on hit

	if (IsActorPlayer(detectionHit.GetActor()) && canSeeHit)
	{
		if (!SDTUtils::IsPlayerPoweredUp(GetWorld())) 
		{
			lastPlayerPosition = detectionHit.GetActor()->GetActorLocation();
			if (IsActorCollectible(bestTarget))
			{
				Cast<ASDTCollectible>(bestTarget)->isTargeted = false;
				AIStateInterrupted();
				bestTarget = detectionHit.GetActor();
			}
			if (bestTarget == nullptr) 
			{
				bestTarget = detectionHit.GetActor();
			}
		}
		else 
		{
			if (IsActorPlayer(bestTarget) || IsActorCollectible(bestTarget)) 
				AIStateInterrupted();
			if(IsActorCollectible(bestTarget))
				Cast<ASDTCollectible>(bestTarget)->isTargeted = false;
			bestTarget = GetFurthestFleeLocation();
		}
	}
	else if ((bestTarget == nullptr || IsActorPlayer(bestTarget)))
	{
		AIStateInterrupted();
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

void ASDTAIController::ResetIfTooMuchCollectibleTargeted() // to deal with a last minute bug
{
	TArray<AActor*> foundCollectibles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASDTCollectible::StaticClass(), foundCollectibles);
	int numberOfTargetedCollectible = 0;
	for (AActor* collectible : foundCollectibles)
	{
		if (Cast<ASDTCollectible>(collectible)->isTargeted == true)
			numberOfTargetedCollectible++;
	}
	if (numberOfTargetedCollectible > 3) {
		UE_LOG(LogTemp, Warning, TEXT("Your message"));
		for (AActor* collectible : foundCollectibles)
		{
			Cast<ASDTCollectible>(collectible)->isTargeted = false;
		}
	}
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
		if(nearestDistance > collectible->GetDistanceTo(selfPawn) && IsActorCollectibleAndAvailable(collectible))
		{
			nearestCollectible = collectible;
			nearestDistance = collectible->GetDistanceTo(selfPawn);
		}
	}
	if (nearestCollectible != nullptr)
		Cast<ASDTCollectible>(nearestCollectible)->isTargeted = true;
	return nearestCollectible;
}

AActor* ASDTAIController::GetFurthestFleeLocation() {

	UWorld* word = GetWorld();
	APawn* selfPawn = GetPawn();
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector directionOfPlayer = (playerCharacter->GetActorLocation() - selfPawn->GetActorLocation()).GetSafeNormal();
	TArray<AActor*> foundFleeLocations;
	UGameplayStatics::GetAllActorsOfClass(word, ASDTFleeLocation::StaticClass(), foundFleeLocations);
	AActor* furthestFleeLocation = nullptr;
	float furthestDistance = 0;

	for (AActor* fleePos : foundFleeLocations)
	{
		FVector directionOfFleePos = (fleePos->GetActorLocation() - selfPawn->GetActorLocation()).GetSafeNormal();
		float dotResult = FVector::DotProduct(directionOfPlayer, directionOfFleePos); //if < 0, not the same direction
		if (fleePos->GetDistanceTo(selfPawn) > furthestDistance && dotResult < 0 )
		{
			furthestFleeLocation = fleePos;
			furthestDistance = fleePos->GetDistanceTo(selfPawn);
		}
	}

	return furthestFleeLocation;
}


bool ASDTAIController::IsActorPlayer(AActor* actor) {
	if (actor != nullptr)
		return (actor->GetName().Contains("Main"));
	return false;
}

bool ASDTAIController::IsActorCollectible(AActor* actor) {
	if (actor != nullptr) 
		return actor->GetName().Contains("Collectible");
	return false;
}

bool ASDTAIController::IsActorCollectibleAndAvailable(AActor* actor) {
	if (actor != nullptr) {
		if (actor->GetName().Contains("Collectible"))
			return !Cast<ASDTCollectible>(actor)->IsOnCooldown() && !Cast<ASDTCollectible>(actor)->isTargeted;
	}
	return false;
} 