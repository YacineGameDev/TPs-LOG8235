// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "PhysicsHelpers.h"


void ASDTAIController::Tick(float deltaTime)
{
	currentSpeed = ComputeSpeed(deltaTime);
	Align();
	MoveForward(currentSpeed, deltaTime);
	DetectObstable();
}

float ASDTAIController::ComputeSpeed(float deltaTime) {
	return currentSpeed + ACCELERATION * deltaTime;
}

void ASDTAIController::MoveForward( float currentSpeed, float deltaTime)
{
	APawn* pawn = GetPawn();
	FVector2D direction = FVector2D(pawn->GetActorForwardVector());
	FVector const pawnPosition(pawn->GetActorLocation());
	FVector2D const displacement = FMath::Min(500.f,currentSpeed) * deltaTime * direction;
	pawn->SetActorLocation(pawnPosition + FVector(displacement, 0.f), true);
}

/*void ASDTAIController::MoveActor(float nSpeed) {
	APawn* pawn = GetPawn();
	float velocity = pawn->GetVelocity().Size() + nSpeed;
	pawn->AddMovementInput(GetActorForwardVector(), velocity);
	if (pawn->GetMovementComponent()->IsExceedingMaxSpeed(V_MAX)) {
		pawn->AddMovementInput(nSpeed * GetActorForwardVector(), -1.0);
	}
	DetectObstable();
}*/

void ASDTAIController::Align() {
}

TArray<struct FHitResult> ASDTAIController::DetectObstable() {

	APawn* pawn = GetPawn();
	UWorld * World = GetWorld();
	PhysicsHelpers physicHelper(World);
	FVector actorLocation = pawn->GetActorLocation();
	FVector endPointRay = actorLocation + pawn->GetActorForwardVector()*250.0f;
	TArray<struct FHitResult> hitResult;
	physicHelper.CastRay(actorLocation, endPointRay, hitResult, true);
	return hitResult;
	
}




