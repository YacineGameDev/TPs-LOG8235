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
	virtual void ComputeImpulsion(FVector impulsionDirection, float impulsionRatio = 0);
	virtual void MoveActor();
	virtual TArray<struct FHitResult> DetectObstacle();
	virtual void AvoidObstacle(FHitResult hit);
	virtual void PursueObstacle( FHitResult hit);

	virtual FVector GetRelativeVector(FVector actorPosition, FVector targetPosition);
	virtual void ComputeStrategy(TArray<struct FHitResult> hitResults);
	/*UFUNCTION()
	void ToggleTurningDirection();*/

private:
	// Distance ou la sphere de l'agent commence a detecter des obstacles (valeur minimum: 0)
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float detection_distance = 40.0f;

	// Vitesse de l'agent (valeur minimum: 0)
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float speed = 250.0f;
	// Rayon de la sphere detection (valeur minimum: 0)
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float sphere_radius = 180.0f;
	bool isTurningRight = false;
};