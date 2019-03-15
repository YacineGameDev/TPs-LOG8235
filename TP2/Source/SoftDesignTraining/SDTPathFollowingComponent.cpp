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

    if (SDTUtils::HasJumpFlag(segmentStart))
    {
		UE_LOG(LogTemp, Warning, TEXT("Your message"));
		AActor* actor = const_cast<AActor*>(Path->GetSourceActor());
		if (actor != nullptr) {

			ACharacter* character = Cast<ACharacter>(actor);
			ASDTAIController* controller = Cast<ASDTAIController>(character->GetController());
			if (controller->AtJumpSegment)
			{
				controller->AtJumpSegment = false;
			} 
			else
			{
				character->Jump();
				controller->AtJumpSegment = true;
			}
			

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

    if (SDTUtils::HasJumpFlag(segmentStart) && FNavMeshNodeFlags(segmentStart.Flags).IsNavLink())
    {

	}
    else
    {
        //Handle normal segments
    }
}

