// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Engine.h"
#include "SDTAIController.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API ASDTAIController : public AAIController
{
    GENERATED_BODY()
public:

	virtual void BeginPlay() override;

    virtual void Tick(float deltaTime) override;
	virtual void ComputeSpeedRatio(bool isAccelerating, FVector vectorToObstacle);
	virtual void MoveActor();
	virtual TArray<struct FHitResult> DetectObstacle();
	virtual void AvoidObstacle(FHitResult hit);
	virtual void PursueObstacle( FHitResult hit);
	virtual FVector GetRelativeDistance(FVector actorPosition, FVector targetPosition);
	virtual void ComputeStrategy(TArray<struct FHitResult> hitResults);
	virtual void displayInfos();

	UFUNCTION()
	virtual void incrementTimer();

private:
	// Distance ou la sphere de l'agent commence a detecter des obstacles (valeur minimum: 0)
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float detection_distance = 100.0f;

	// Vitesse de l'agent (valeur minimum: 0)
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float speed = 300.0f;
	// Rayon de la sphere detection (valeur minimum: 0)
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float sphere_radius = 140.0f;

	FTimerHandle timerHandle;
	int timer = 0; //timer in seconds

	float speedRatio = 1;
	bool isTurningRight = true;
};