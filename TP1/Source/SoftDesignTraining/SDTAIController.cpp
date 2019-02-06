// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "PhysicsHelpers.h"


void ASDTAIController::Tick(float deltaTime)
{
	Align();
	MoveActor();
	TArray<struct FHitResult> hitResult = DetectObstacle();
	AvoidObstacle(hitResult);
}


void ASDTAIController::MoveActor() {
	APawn* pawn = GetPawn();
	FVector direction = pawn->GetActorForwardVector();
	float velocity = pawn->GetVelocity().Size();
	if (velocity < MAX_SPEED) {
		pawn->AddMovementInput(direction, speedRatio);
	}
}

void ASDTAIController::Align() {
}

TArray<struct FHitResult> ASDTAIController::DetectObstacle() {

	APawn* pawn = GetPawn();
	UWorld * World = GetWorld();
	PhysicsHelpers physicHelper(World);
	FVector actorLocation = pawn->GetActorLocation();
	FVector endPointRay = actorLocation + pawn->GetActorForwardVector()*DETECTION_DISTANCE;
	TArray<struct FHitResult> hitResult;
	physicHelper.CastRay(actorLocation, endPointRay, hitResult, true);
	return hitResult;
}

FVector ASDTAIController::GetRelativeDistance(FVector actorPosition, FVector targetPosition) {
	return targetPosition - actorPosition;
}

void ASDTAIController::ComputeSpeedRatio(bool isAccelerating, float distanceFromObstacle) {
	if (isAccelerating) {
		speedRatio = speedRatio >= 1.0 ? 1.0: speedRatio * 1.005;
	}
	else {
		speedRatio = distanceFromObstacle / (2 * DETECTION_DISTANCE);
	}
	
}

void ASDTAIController::AvoidObstacle(TArray<struct FHitResult> hitResults) {
	//FHitResult* OutSweepHitResult;
	//ETeleportType Teleport;

	if (hitResults.Num() != 0) {
		APawn* pawn = GetPawn();
		FVector actorPosition = pawn->GetActorLocation();
		FVector obstaclePosition = hitResults.GetData()->GetActor()->GetActorLocation();
		FVector2D relativeDistance = FVector2D(GetRelativeDistance(actorPosition, obstaclePosition));
		ComputeSpeedRatio(false, relativeDistance.Size());

		if (relativeDistance.Size() < 300.0f) {
			FRotator deltaRotation = FRotator(0, 1, 0);
			pawn->AddActorWorldRotation(deltaRotation, false);
		}

	}
	else {
		ComputeSpeedRatio(true);
	}
}




