// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "PhysicsHelpers.h"
#include "DrawDebugHelpers.h"

void ASDTAIController::Tick(float deltaTime)
{
	MoveActor();
	TArray<struct FHitResult> hitResultsSphere = DetectObstacle();
	TArray<struct FHitResult> hitResultsRay = DetectFrontObstacle();
	if (hitResultsSphere.Num() != 0) {
		AvoidObstacle(hitResultsSphere);
	}
	if (hitResultsRay.Num() != 0) {
		FVector2D relativeDistance = FVector2D(GetRelativeDistance(GetPawn()->GetActorLocation(), hitResultsRay.GetData()->GetActor()->GetActorLocation()));
		ComputeSpeedRatio(false, relativeDistance.Size());
	}
	else {
		ComputeSpeedRatio(true);

	}
	UE_LOG(LogTemp, Display, TEXT("**************************"));
}


void ASDTAIController::MoveActor() {
	APawn* pawn = GetPawn();
	FVector direction = pawn->GetActorForwardVector();
	UE_LOG(LogTemp, Warning, TEXT("ACTOR: %s"), *pawn->GetName());
	float velocity = pawn->GetVelocity().Size();
	if (velocity < MAX_SPEED) {
		pawn->AddMovementInput(direction, speedRatio);
	}
}

TArray<struct FHitResult> ASDTAIController::DetectFrontObstacle() {

	APawn* pawn = GetPawn();
	UWorld * World = GetWorld();
	PhysicsHelpers physicHelper(World);
	FVector actorLocation = pawn->GetActorLocation();
	FVector endPointRay = actorLocation + pawn->GetActorForwardVector() * DETECTION_DISTANCE;
	TArray<struct FHitResult> hitResult;
	physicHelper.CastRay(actorLocation, endPointRay, hitResult, true);
	return hitResult;
}

TArray<struct FHitResult> ASDTAIController::DetectObstacle() {

	APawn* pawn = GetPawn();
	FVector actorLocation = pawn->GetActorLocation();

	FVector Origin;
	FVector BoundsExtent;
	pawn->GetActorBounds(false, Origin, BoundsExtent);

	TArray<FHitResult> OutHits;

	// start and end locations
	FVector Detectionstart = pawn->GetActorLocation() + (pawn->GetActorForwardVector() * SPHERE_OFFSET);
	//Detectionstart.Z -= BoundsExtent.Z;
	// create a collision sphere
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(SPHERE_RADIUS);
	// draw collision sphere
	DrawDebugSphere(GetWorld(), Detectionstart, MyColSphere.GetSphereRadius(), 20, FColor::Purple, false, -1,0);

	// check if something got hit in the sweep
	GetWorld()->SweepMultiByChannel(OutHits, Detectionstart, Detectionstart, FQuat::Identity, ECC_Pawn, MyColSphere);
	return OutHits;
}

void ASDTAIController::ComputeSpeedRatio(bool isAccelerating, float distanceFromObstacle) {
	if (isAccelerating) {
		speedRatio = speedRatio >= 1.0 ? 1.0: speedRatio * 1.01;
	}
	else {
		speedRatio = 0.5;
	}
	
}

FVector ASDTAIController::GetRelativeDistance(FVector actorPosition, FVector targetPosition) {
	return targetPosition - actorPosition;
}

void ASDTAIController::AvoidObstacle(TArray<struct FHitResult> hitResults) {
	for (auto& Hit : hitResults)
	{

		// just for dev
		 if (Hit.Actor->GetName().Contains("Wall") || Hit.Actor->GetName().Contains("Death") || Hit.Actor->GetName().Contains("Main")) {
			APawn* pawn = GetPawn();
			FVector2D const turnDirection = FVector2D(FVector::CrossProduct(pawn->GetActorUpVector(), Hit.Normal));
			FVector tempCo = FVector::CrossProduct(pawn->GetActorForwardVector(), FVector(turnDirection, 0.0f));
			//UE_LOG(LogTemp, Warning, TEXT("test: %s"), *Hit.Actor->GetName());
			float W = FMath::Sqrt(2) + FVector::DotProduct(pawn->GetActorForwardVector(), FVector(turnDirection, 0.0f));
			FQuat orientationQuat = FQuat(tempCo.X, tempCo.Y, tempCo.Z, W);
			orientationQuat.Normalize();
			FQuat rotateTo = (orientationQuat) / 7.0f;
			FRotator deltaRotation = FRotator(rotateTo);

			pawn->AddActorWorldRotation(deltaRotation, false);
		}

	}
}




