// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"

void ASDTAIController::Tick(float deltaTime)
{
	float nSpeed = ComputeSpeed(deltaTime);
	Align();
	MoveActor(nSpeed);
}

float ASDTAIController::ComputeSpeed(float deltaTime) {
	APawn* pawn = GetPawn();
	return ACCELERATION * deltaTime;
}

void ASDTAIController::MoveActor(float nSpeed) {
	APawn* pawn = GetPawn();
	pawn->AddMovementInput(GetActorForwardVector(), nSpeed);
	if (pawn->GetMovementComponent()->IsExceedingMaxSpeed(V_MAX)) {
		pawn->AddMovementInput(nSpeed * GetActorForwardVector(), -1.0);
	}

}

void ASDTAIController::Align() {

}




