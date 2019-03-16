// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTPathFollowingComponent.h"
#include "SDTUtils.h"
#include "SDTAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DrawDebugHelpers.h"

USDTPathFollowingComponent::USDTPathFollowingComponent(const FObjectInitializer& ObjectInitializer)
{

}

void USDTPathFollowingComponent::FollowPathSegment(float DeltaTime)
{

    const TArray<FNavPathPoint>& points = Path->GetPathPoints();
    const FNavPathPoint& segmentStart = points[MoveSegmentStartIndex];
	const FNavPathPoint& segmentEnd = points[MoveSegmentStartIndex + 1];
	FVector direction = (segmentEnd.Location - segmentStart.Location).GetSafeNormal();

	AActor* actor = const_cast<AActor*>(Path->GetSourceActor());
	ACharacter* Character = Cast<ACharacter>(actor);
	ASDTAIController* controller = Cast<ASDTAIController>(Character->GetController());
	auto MovementComponent = Character->GetCharacterMovement();

    if (SDTUtils::HasJumpFlag(segmentStart))
    {
		if (controller->JumpProgress >= 0)
			controller->JumpProgress -= DeltaTime;
		MovementComp->RequestPathMove(direction);
		MovementComponent->Velocity.X = 420 * direction.X;
		MovementComponent->Velocity.Y = 420 * direction.Y;
		MovementComponent->JumpZVelocity = 600.0f;
		MovementComponent->DoJump(false);
		controller->AtJumpSegment = true;
	}
    else
    {
		Super::FollowPathSegment(DeltaTime);
		controller->AtJumpSegment = false;
		controller->JumpProgress = 1.0f;
	}
}

void USDTPathFollowingComponent::SetMoveSegment(int32 segmentStartIndex)
{
    Super::SetMoveSegment(segmentStartIndex);

    const TArray<FNavPathPoint>& points = Path->GetPathPoints();
    const FNavPathPoint& segmentStart = points[MoveSegmentStartIndex];

	AActor* actor = const_cast<AActor*>(Path->GetSourceActor());
	ACharacter* Character = Cast<ACharacter>(actor);
	ASDTAIController* controller = Cast<ASDTAIController>(Character->GetController());
	auto MovementComponent = Character->GetCharacterMovement();

    if (SDTUtils::HasJumpFlag(segmentStart) && FNavMeshNodeFlags(segmentStart.Flags).IsNavLink())
    {
		MovementComponent->SetMovementMode(MOVE_Falling);
	}
    else
    {
        //Handle normal segments
		MovementComponent->SetMovementMode(MOVE_Walking);
    }
}

