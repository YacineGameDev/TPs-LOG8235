// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SOFTDESIGNTRAINING_API FpsManager
{
public:
	static FpsManager* GetInstance();
	bool canExecute(uint8 taskId);
	~FpsManager();

private:
	static FpsManager* m_Instance;
	int currentFrame = 0;
	float timeLeft = 0.033;
	FpsManager();
};
