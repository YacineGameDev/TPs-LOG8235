// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "PhysicsHelpers.h"
#include "DrawDebugHelpers.h"
#include "SoftDesignTrainingMainCharacter.h"
#include "SoftDesignTrainingCharacter.h"
#include "Kismet/GameplayStatics.h"


void ASDTAIController::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ASDTAIController::incrementTimer, 1.0f, true);

}

void ASDTAIController::Tick(float deltaTime)
{
	displayInfos();
	MoveActor();
	TArray<struct FHitResult> hitResultsSphere = DetectObstacle();
	if (hitResultsSphere.Num() != 0) {
		ComputeStrategy(hitResultsSphere);
	}
	else {
		ComputeSpeedRatio(true, FVector::ZeroVector);
	}
}


void ASDTAIController::MoveActor() {
	APawn* pawn = GetPawn();
	FVector direction = pawn->GetActorForwardVector();
	float velocity = pawn->GetVelocity().Size();
	if (velocity < speed) {
		pawn->AddMovementInput(direction, speedRatio);
	}
}

void ASDTAIController::incrementTimer() {
	timer++;
}


void ASDTAIController::displayInfos() {
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASoftDesignTrainingCharacter::StaticClass(), FoundActors);
	
	FString timerMinutes;
	FString timerSeconds;

	if (floor(timer / 60) >= 10)
		timerMinutes = FString::FromInt(floor(timer / 60));
	else 
		timerMinutes = FString::Printf(TEXT("0")) + FString::FromInt(floor(timer / 60));

	if (timer % 60 >= 10)
		timerSeconds = FString::FromInt(timer % 60);
	else
		timerSeconds = FString::Printf(TEXT("0")) + FString::FromInt(timer % 60);


	FString message = FString::Printf(TEXT("Temps Ecoule: ")) + timerMinutes + FString::Printf(TEXT(":")) + timerSeconds + FString::Printf(TEXT("\n"));

	for (int i = 0; i < FoundActors.Num(); i++) {
		ASoftDesignTrainingCharacter* player = Cast<ASoftDesignTrainingCharacter>(FoundActors[i]);

		FString name = player->GetName();
		int pawnScore = player->GetScore();
		int deatToll = player->GetDeathToll();

		message += name + FString::Printf(TEXT(" Details:\nScore: ")) + FString::FromInt(pawnScore) + FString::Printf(TEXT("\nNb Morts: ")) + FString::FromInt(deatToll);

		if (i != FoundActors.Num() - 1) {
			message += FString::Printf(TEXT("\n"));
		}
	}


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Red, message);

	}

}

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

void ASDTAIController::ComputeSpeedRatio(bool isAccelerating, FVector vectorToObstacle) {
	if (isAccelerating) {
		speedRatio = speedRatio >= 1.0 ? 1.0: speedRatio * 1.01;
	}
	else {
		//speedRatio = speedRatio >= 0.1 ? speedRatio * 0.975 : speedRatio;
		APawn* pawn = GetPawn();
		pawn->AddMovementInput(vectorToObstacle.GetSafeNormal(), speedRatio/3);
	}
	
}

FVector ASDTAIController::GetRelativeDistance(FVector actorPosition, FVector targetPosition) {
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
	UE_LOG(LogTemp, Warning, TEXT("Test : %s"), *hit.Actor->GetName());
	GetWorld()->LineTraceSingleByObjectType(outhit, location, location + hit.GetActor()->GetActorLocation(), ObjectQueryParams);
	/*if (outhit.) {
		UE_LOG(LogTemp, Warning, TEXT("No : %s"), *hit.Actor->GetName());
	}*/
}

void ASDTAIController::AvoidObstacle(FHitResult hit) {
		APawn* pawn = GetPawn();
		FVector turnDirection;
		if (isTurningRight) 
			turnDirection = FVector::CrossProduct(hit.Normal, pawn->GetActorUpVector());
		else
			turnDirection = FVector::CrossProduct(pawn->GetActorUpVector(), hit.Normal);

		FVector rotationAxis = FVector::CrossProduct(pawn->GetActorForwardVector(), turnDirection);

		pawn->AddActorWorldRotation(FQuat(rotationAxis, 0.05), false);

		FVector vectorToObstacle = GetRelativeDistance(pawn->GetActorLocation(), hit.Actor->GetActorLocation());
		//DrawDebugLine(GetWorld(), pawn->GetActorLocation(), pawn->GetActorLocation() + vectorToObstacle, FColor::Red, false, -1, 0, 5.0f);
		//FVector impulseDirection = FVector
		ComputeSpeedRatio(false, hit.Normal);
}
// Fill out your copyright notice in the Description page of Project Settings.