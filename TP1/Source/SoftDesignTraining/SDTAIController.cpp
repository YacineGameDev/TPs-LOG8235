// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "PhysicsHelpers.h"


void ASDTAIController::Tick(float deltaTime)
{
	float computeSpeed = ComputeSpeed(deltaTime);
	Align();
	MoveActor(computeSpeed);
	TArray<struct FHitResult> hitResult = DetectObstacle();
	AvoidObstacle(hitResult);
}

float ASDTAIController::ComputeSpeed(float deltaTime) {
	return acceleration * deltaTime;
}

void ASDTAIController::MoveActor(float computeSpeed) {
	APawn* pawn = GetPawn();
	FVector direction = pawn->GetActorForwardVector();
	float velocity = pawn->GetVelocity().Size() + computeSpeed;
	if (velocity < MAX_SPEED) {
		pawn->AddMovementInput(direction, velocity);
	}
}

void ASDTAIController::Align() {
}

TArray<struct FHitResult> ASDTAIController::DetectObstacle() {

	APawn* pawn = GetPawn();
	UWorld * World = GetWorld();
	PhysicsHelpers physicHelper(World);
	FVector actorLocation = pawn->GetActorLocation();
	FVector endPointRay = actorLocation + pawn->GetActorForwardVector()*500.0f;
	TArray<struct FHitResult> hitResult;
	physicHelper.CastRay(actorLocation, endPointRay, hitResult, true);
	return hitResult;
}

FVector ASDTAIController::GetRelativeDistance(FVector actorPosition, FVector targetPosition) {
	return targetPosition - actorPosition;
}

void ASDTAIController:: ComputeAcceleration(float distanceFromObstable) {
	float ratio = 1;
	if (distanceFromObstable < DECELERATION_DISTANCE) {
		ratio = distanceFromObstable / DECELERATION_DISTANCE;
		acceleration *= -ratio;
	}
	acceleration = MAX_ACCELERATION;
}

void ASDTAIController::AvoidObstacle(TArray<struct FHitResult> hitResults) {
	if (hitResults.GetData() != nullptr) {
		APawn* pawn = GetPawn();
		FVector actorPosition = pawn->GetActorLocation();
		FVector obstaclePosition = hitResults.GetData()->GetActor()->GetActorLocation();
		FVector2D relativeDistance = FVector2D(GetRelativeDistance(actorPosition, obstaclePosition));
		ComputeAcceleration(relativeDistance.Size());
		FRotator deltaRotation = FRotator(0,0,0);
		//FHitResult* OutSweepHitResult;
		//ETeleportType Teleport;
		pawn->AddActorWorldRotation(deltaRotation, false);
	}
}




