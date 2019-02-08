// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "SDTAIController.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API ASDTAIController : public AAIController
{
    GENERATED_BODY()
public:
    virtual void Tick(float deltaTime) override;
	virtual void ComputeSpeedRatio(bool isAccelerating, float distanceFromObstacle = 0);

	virtual void MoveActor();
	virtual TArray<struct FHitResult> DetectObstacle();
	virtual void AvoidObstacle(TArray<struct FHitResult> hitResults);
	virtual TArray<struct FHitResult> DetectFrontObstacle();
	virtual FVector GetRelativeDistance(FVector actorPosition, FVector targetPosition);
private:

	float const MAX_SPEED = 250.0f;
	float const SPHERE_RADIUS = 140;
	float const SPHERE_OFFSET = 90.0f;
	float const DETECTION_DISTANCE = 300.0f;

	float speedRatio = 1;

	// to remove
	// float acceleration = MAX_ACCELERATION;


};