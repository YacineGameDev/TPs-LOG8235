// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SoftDesignTrainingCharacter.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SOFTDESIGNTRAINING_API AiAgentGroupManager
{

public:
	static AiAgentGroupManager* GetInstance();
	static void Destroy();

	void RegisterAIAgent(ASoftDesignTrainingCharacter* character);
	void UnregisterAIAgent(ASoftDesignTrainingCharacter* character);

	
private:
	AiAgentGroupManager();
	static AiAgentGroupManager* m_Instance;

	TArray<ASoftDesignTrainingCharacter*> m_registeredAgents;
};
