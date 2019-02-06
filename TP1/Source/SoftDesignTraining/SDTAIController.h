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
	float ComputeSpeed(float deltaTime);
	//virtual void MoveActor(float nSpeed);
	virtual void MoveForward(float speed, float deltaTime);
	virtual void Align();
	virtual TArray<struct FHitResult> DetectObstable();
private:
	float const ACCELERATION = 40.0f;
	float const MAX_SPEED = 500.f;
	bool hasBegun = false;
	float currentSpeed = 0.0f;
};
