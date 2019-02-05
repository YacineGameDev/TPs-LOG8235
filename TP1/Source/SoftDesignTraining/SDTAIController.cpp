// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "PhysicsHelpers.h"


void ASDTAIController::Tick(float deltaTime)
{
	float nSpeed = ComputeSpeed(deltaTime);
	Align();
	MoveForward(nSpeed, deltaTime);
}

float ASDTAIController::ComputeSpeed(float deltaTime) {
	APawn* pawn = GetPawn();
	return ACCELERATION * deltaTime;
}

void ASDTAIController::MoveForward( float nSpeed, float deltaTime)
{
	APawn* pawn = GetPawn();
	float velocity = pawn->GetVelocity().Size() + nSpeed;
	FVector2D direction = FVector2D(pawn->GetActorForwardVector());
	FVector const pawnPosition(pawn->GetActorLocation());
	FVector2D const displacement =  velocity * deltaTime * direction;
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

void ASDTAIController::DetectObstable() {

	APawn* pawn = GetPawn();
	//FVector testDirection = pawn->GetActorForwardVector();
	UWorld * World = GetWorld();
	PhysicsHelpers physicHelper(World);
	FVector actorLocation = GetPawn()->GetActorLocation();
	FVector endPointRay = GetPawn()->GetActorForwardVector()*300.0f;
	TArray<struct FHitResult> hitResult;
	physicHelper.CastRay(actorLocation, endPointRay, hitResult, true);
	/*if (hitResult.Num() > 0) {
		pawn->SetActorRotation(testDirection.ToOrientationQuat());
	}*/
}




