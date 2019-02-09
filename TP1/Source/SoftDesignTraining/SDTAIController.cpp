// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "PhysicsHelpers.h"
#include "SDTCollectible.h"
#include "DrawDebugHelpers.h"
#include "SoftDesignTrainingMainCharacter.h"

void ASDTAIController::Tick(float deltaTime)
{
	if (GetPawn()->GetName().Contains("BP_SDTAICharacter2")) {
		MoveActor();
		TArray<struct FHitResult> hitResultsSphere = DetectObstacle();
		if (hitResultsSphere.Num() != 0) {
			ComputeStrategy(hitResultsSphere);
		}
	}
}

void ASDTAIController::MoveActor() {
	APawn* pawn = GetPawn();
	FVector direction = pawn->GetActorForwardVector();
	ComputeImpulsion(direction, 1.0f);
}

/*void ASDTAIController::ToggleTurningDirection() {
	if (FMath::Rand() % 2) {
	}
	isTurningRight = !isTurningRight;
	UE_LOG(LogTemp, Display, TEXT("!!!!!!!!!!!!!!!!!!!!!TEST!!!!!!!!!!!!!!!!!!!!!!!!"));

}*/

void ASDTAIController::ComputeStrategy(TArray<struct FHitResult> hitResults) {
	for (auto& Hit : hitResults)
	{
		if (Hit.Actor->GetName().Contains("Wall") || Hit.Actor->GetName().Contains("Death")) {
			//UE_LOG(LogTemp, Warning, TEXT("Wall or Death: %s"), *Hit.Actor->GetName());
			AvoidObstacle(Hit);
		}
		else if (Hit.Actor->GetName().Contains("Main")) {
			ASoftDesignTrainingMainCharacter* player = Cast<ASoftDesignTrainingMainCharacter>(Hit.Actor.Get());

			if (player->IsPoweredUp()) {
				// RUUUUN!!!
				//UE_LOG(LogTemp, Warning, TEXT("Player PoweredUp: %s"), *Hit.Actor->GetName());
				AvoidObstacle(Hit);
			}
			else {
				//UE_LOG(LogTemp, Warning, TEXT("Player NotPowered : %s"), *Hit.Actor->GetName());
				PursueObstacle(Hit);
			}
		}
		else if (Hit.Actor->GetName().Contains("Collectible")) {
			//UE_LOG(LogTemp, Warning, TEXT("Collectible : %s"), *Hit.Actor->GetName());
			ASDTCollectible* pickUp = Cast< ASDTCollectible>(Hit.Actor.Get());
			if (!pickUp->IsOnCooldown())
				PursueObstacle(Hit);
		}

	}
}

TArray<struct FHitResult> ASDTAIController::DetectObstacle() {

	APawn* pawn = GetPawn();
	TArray<FHitResult> OutHits;
	FVector Detectionstart = pawn->GetActorLocation() + (pawn->GetActorForwardVector() * detection_distance);


	//Setting up the collision shape
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(sphere_radius);

	//Search only for pawns
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel3);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel4);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel5);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

	//Ignore ourselves
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	GetWorld()->SweepMultiByObjectType(OutHits, Detectionstart, Detectionstart, FQuat::Identity, ObjectQueryParams, CollisionShape, CollisionQueryParams);
	
	DrawDebugSphere(GetWorld(), Detectionstart, CollisionShape.GetSphereRadius(), 20, FColor::Purple, false, -1,0);

	//GetWorld()->SweepMultiByChannel(OutHits, Detectionstart, Detectionstart, FQuat::Identity, ECC_Pawn, MyColSphere);
	return OutHits;
}

void ASDTAIController::ComputeImpulsion(FVector impulsionDirection, float impulsionRatio) {
	APawn* pawn = GetPawn();
	float velocity = pawn->GetVelocity().Size();
	if (velocity < speed) {
		pawn->AddMovementInput(impulsionDirection.GetSafeNormal(), impulsionRatio);
	}
}

FVector ASDTAIController::GetRelativeVector(FVector actorPosition, FVector targetPosition) {
	return targetPosition - actorPosition;
}

void ASDTAIController::PursueObstacle(FHitResult hit) {
	APawn* pawn = GetPawn();
	FVector location = pawn->GetActorLocation();

	FHitResult outhit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel3);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel4);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel5);
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.AddIgnoredActor(this->GetCharacter());

	GetWorld()->LineTraceSingleByObjectType(outhit, location, hit.GetActor()->GetActorLocation(), ObjectQueryParams,CollisionQueryParams);
	DrawDebugLine(GetWorld(), location, hit.GetActor()->GetActorLocation(), FColor::Red, false, -1, 0, 5.0f);

	if (outhit.GetActor() == hit.GetActor()) {
		FVector turnDirection = -FVector::CrossProduct(hit.Normal, pawn->GetActorUpVector());
		FVector rotationAxis = FVector::CrossProduct(turnDirection, pawn->GetActorForwardVector());
		pawn->AddActorWorldRotation(FQuat(rotationAxis, 0.019), false);
		ComputeImpulsion(GetRelativeVector(location, hit.Actor->GetActorLocation()), 1.0f);
	}
}

void ASDTAIController::AvoidObstacle(FHitResult hit) {
		APawn* pawn = GetPawn();
		FVector turnDirection;
		if (isTurningRight) 
			turnDirection = FVector::CrossProduct(hit.Normal, pawn->GetActorUpVector());
		else
			turnDirection = FVector::CrossProduct(pawn->GetActorUpVector(), hit.Normal);
		FVector rotationAxis = FVector::CrossProduct(pawn->GetActorForwardVector(), turnDirection);
		if (!hit.Actor->GetName().Contains("Main")) {
			pawn->AddActorWorldRotation(FQuat(rotationAxis, 0.02), false);
		}
		FVector vectorToObstacle = GetRelativeVector(pawn->GetActorLocation(), hit.Actor->GetActorLocation());
		//DrawDebugLine(GetWorld(), pawn->GetActorLocation(), pawn->GetActorLocation() + vectorToObstacle, FColor::Red, false, -1, 0, 5.0f);
		//FVector impulseDirection = FVector
		ComputeImpulsion(hit.Normal, 0.3);
}