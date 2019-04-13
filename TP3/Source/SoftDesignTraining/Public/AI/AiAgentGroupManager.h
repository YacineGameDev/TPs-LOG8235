// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SoftDesignTrainingCharacter.h"
#include "CoreMinimal.h"
#include "TargetPosition.h"

class SOFTDESIGNTRAINING_API AiAgentGroupManager
{

public:
	static AiAgentGroupManager* GetInstance();
	static void Destroy();

	void RegisterAIAgent(APawn* character);
	void UnregisterAIAgent(APawn* character);

	void initTargetPos(ACharacter* character);
	void FreeLocation(APawn* character, int idx);
	FVector allocateTargetPos(ACharacter* character, int & idx);

	TArray<APawn*> m_registeredAgents;
	TArray<ATargetPosition*> targetPositions;
	
private:
	AiAgentGroupManager();
	static AiAgentGroupManager* m_Instance;
	TMap<FString, FVector> m_targetMaps;

};
