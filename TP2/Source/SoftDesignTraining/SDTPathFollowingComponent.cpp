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

	// UE_LOG(LogTemp, Warning, TEXT("InAir: %s"), (controller->InAir ? TEXT("True") : TEXT("False")));
	UE_LOG(LogTemp, Warning, TEXT("JumpZVelocity: %f"), MovementComponent->Velocity.Z);
	UE_LOG(LogTemp, Warning, TEXT("IsNavLink: %s"), (FNavMeshNodeFlags(segmentStart.Flags).IsNavLink() ? TEXT("True") : TEXT("False")));

    if (SDTUtils::HasJumpFlag(segmentStart)) /*&& controller->AtJumpSegment*/
    {
		if (MovementComponent->Velocity.Z >= 0 && !controller->Landing)
		{
			MovementComp->RequestPathMove(direction);
			MovementComponent->Velocity.X = 400 * direction.X;
			MovementComponent->Velocity.Y = 400 * direction.Y;
			MovementComponent->JumpZVelocity = 600.0f;
			MovementComponent->DoJump(false);
			//UE_LOG(LogTemp, Warning, TEXT("Your message1"));


		}
		else 
		{
			controller->Landing = true;
			//UE_LOG(LogTemp, Warning, TEXT("Your message2"));
		}
	}
    else
    {
		Super::FollowPathSegment(DeltaTime);
	}
}

void USDTPathFollowingComponent::SetMoveSegment(int32 segmentStartIndex)
{
    Super::SetMoveSegment(segmentStartIndex);

    const TArray<FNavPathPoint>& points = Path->GetPathPoints();

    const FNavPathPoint& segmentStart = points[MoveSegmentStartIndex];
	const FNavPathPoint& segmentEnd = points[MoveSegmentStartIndex + 1];
	//segmentEnd

	AActor* actor = const_cast<AActor*>(Path->GetSourceActor());
	ACharacter* Character = Cast<ACharacter>(actor);
	ASDTAIController* controller = Cast<ASDTAIController>(Character->GetController());
	auto MovementComponent = Character->GetCharacterMovement();

    if (SDTUtils::HasJumpFlag(segmentStart) && FNavMeshNodeFlags(segmentStart.Flags).IsNavLink())
    {
		MovementComponent->SetMovementMode(MOVE_Falling);
		controller->Landing = false;
		//controller->AtJumpSegment = true;
	}
    else
    {
        //Handle normal segments
		MovementComponent->SetMovementMode(MOVE_Walking);
    }
}

