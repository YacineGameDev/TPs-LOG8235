// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "PhysicsHelpers.h"
#include "DrawDebugHelpers.h"

void ASDTAIController::Tick(float deltaTime)
{
	APawn* pawn = GetPawn();
	// if condition just for dev to make it easier to focus on one Agent.
	if (pawn->GetName() == "BP_SDTAICharacter2") {
		MoveActor();
	}
}


void ASDTAIController::MoveActor() {
	APawn* pawn = GetPawn();
	FVector direction = pawn->GetActorForwardVector();
	UE_LOG(LogTemp, Warning, TEXT("ACTOR: %s"), *pawn->GetName());
	float velocity = pawn->GetVelocity().Size();
	if (velocity < MAX_SPEED) {
		pawn->AddMovementInput(direction, speedRatio);
	}

	TArray<struct FHitResult> hitResults = DetectObstacle();
	if (hitResults.Num() != 0) {
		AvoidObstacle(hitResults);
	}
	else {
		ComputeSpeedRatio(true);
	}
	UE_LOG(LogTemp, Display, TEXT("**************************"));

}

TArray<struct FHitResult> ASDTAIController::DetectObstacle() {

	APawn* pawn = GetPawn();
	FVector actorLocation = pawn->GetActorLocation();

	FVector Origin;
	FVector BoundsExtent;
	pawn->GetActorBounds(false, Origin, BoundsExtent);

	TArray<FHitResult> OutHits;

	// start and end locations
	FVector Detectionstart = pawn->GetActorLocation() + (pawn->GetActorForwardVector() * 200.0f);
	Detectionstart.Z -= BoundsExtent.Z;
	// create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(100.0f);
	// draw collision sphere
	// DrawDebugSphere(GetWorld(), SweepStart, MyColSphere.GetSphereRadius(), 20, FColor::Purple, false, -1,0);

	// check if something got hit in the sweep
	GetWorld()->SweepMultiByChannel(OutHits, Detectionstart, Detectionstart, FQuat::Identity, ECC_WorldStatic, MyColSphere);

	return OutHits;

}


void ASDTAIController::ComputeSpeedRatio(bool isAccelerating, float distanceFromObstacle) {
	if (isAccelerating) {
		speedRatio = speedRatio >= 1.0 ? 1.0: speedRatio * 1.005;
	}
	else {
		speedRatio = distanceFromObstacle / (2 * DETECTION_DISTANCE);
	}
	
}

FVector ASDTAIController::GetRelativeDistance(FVector actorPosition, FVector targetPosition) {
	return targetPosition - actorPosition;
}

void ASDTAIController::AvoidObstacle(TArray<struct FHitResult> hitResults) {
	//FHitResult* OutSweepHitResult;
	//ETeleportType Teleport;
	UE_LOG(LogTemp, Warning, TEXT("I DETECT !!!!!!"));
	for (auto& Hit : hitResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Result: %s"), *Hit.Actor->GetName());
	}

	APawn* pawn = GetPawn();
	FVector actorPosition = pawn->GetActorLocation();
	FVector obstaclePosition = hitResults.GetData()->GetActor()->GetActorLocation();
	FVector2D relativeDistance = FVector2D(GetRelativeDistance(actorPosition, obstaclePosition));
	ComputeSpeedRatio(false, relativeDistance.Size());

	//FRotator deltaRotation = FRotator(0, 1, 0);
	//pawn->AddActorWorldRotation(deltaRotation, false);

	/*if (relativeDistance.Size() < 300.0f) {
		FRotator deltaRotation = FRotator(0, 1, 0);
		pawn->AddActorWorldRotation(deltaRotation, false);
	}*/
}




