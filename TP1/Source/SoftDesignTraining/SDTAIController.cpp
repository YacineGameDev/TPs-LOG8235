// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "PhysicsHelpers.h"
#include "DrawDebugHelpers.h"

void ASDTAIController::Tick(float deltaTime)
{
	MoveActor();
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
	FVector Detectionstart = pawn->GetActorLocation() + (pawn->GetActorForwardVector() * DETECTION_DISTANCE);
	//Detectionstart.Z -= BoundsExtent.Z;
	// create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(150.0f);
	// draw collision sphere
	DrawDebugSphere(GetWorld(), Detectionstart, MyColSphere.GetSphereRadius(), 20, FColor::Purple, false, -1,0);

	// check if something got hit in the sweep
	GetWorld()->SweepMultiByChannel(OutHits, Detectionstart, Detectionstart, FQuat::Identity, ECC_Pawn, MyColSphere);


	return OutHits;

}


void ASDTAIController::ComputeSpeedRatio(bool isAccelerating, float distanceFromObstacle) {
	if (isAccelerating) {
		speedRatio = speedRatio >= 1.0 ? 1.0: speedRatio * 1.005;
	}
	else {
		speedRatio = 0.20f;
	}
	
}

FVector ASDTAIController::GetRelativeDistance(FVector actorPosition, FVector targetPosition) {
	return targetPosition - actorPosition;
}

void ASDTAIController::AvoidObstacle(TArray<struct FHitResult> hitResults) {
	for (auto& Hit : hitResults)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Result: %s"), *Hit.Actor->GetName());

		// just for dev
		 if (Hit.Actor->GetName().Contains("Wall") || Hit.Actor->GetName().Contains("Death") || Hit.Actor->GetName().Contains("Main")) {
			APawn* pawn = GetPawn();
			FVector actorPosition = pawn->GetActorLocation();
			FVector obstaclePosition = hitResults.GetData()->GetActor()->GetActorLocation();
			FVector2D relativeDistance = FVector2D(GetRelativeDistance(actorPosition, obstaclePosition));
			ComputeSpeedRatio(false, relativeDistance.Size());

			FVector2D const turnDirection = FVector2D(FVector::CrossProduct(pawn->GetActorUpVector(), Hit.ImpactNormal));
			FVector tempCo = FVector::CrossProduct(pawn->GetActorForwardVector(), FVector(turnDirection, 0.0f));
			float W = FMath::Sqrt(2) + FVector::DotProduct(pawn->GetActorForwardVector(), FVector(turnDirection, 0.0f));
			FQuat orientationQuat = FQuat(tempCo.X,tempCo.Y,tempCo.Z, W);
			orientationQuat.Normalize();
			FQuat rotateTo = (orientationQuat) / 5.0f;

			FRotator deltaRotation = FRotator(rotateTo);
			pawn->AddActorWorldRotation(deltaRotation, false);
		}

	}
}




