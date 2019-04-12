// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "TargetPosition.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API ATargetPosition : public ATargetPoint
{
	GENERATED_BODY()

	ATargetPosition();

public:
	bool isFree;
	
};
