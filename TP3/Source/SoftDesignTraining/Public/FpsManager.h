// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define AVERAGE_EXEC_TIME 0.0002f
#define BUDGET 0.025f

/**
 * 
 */
class SOFTDESIGNTRAINING_API FpsManager
{

public:
	static FpsManager* GetInstance();
	bool canExecute();
	void Destroy();

private:
	FpsManager();
	static FpsManager* m_FPS_Instance;
	int currentFrame = 0;
	float timeLeft = 0.033;
};
