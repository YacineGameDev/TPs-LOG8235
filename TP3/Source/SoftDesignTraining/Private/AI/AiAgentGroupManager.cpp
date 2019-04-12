// Fill out your copyright notice in the Description page of Project Settings.
#include "SDTUtils.h"
#include "SoftDesignTraining.h"
#include "AiAgentGroupManager.h"


AiAgentGroupManager* AiAgentGroupManager::m_Instance;

AiAgentGroupManager::AiAgentGroupManager()
{
}

AiAgentGroupManager* AiAgentGroupManager::GetInstance()
{
	if (!m_Instance)
	{
		m_Instance = new AiAgentGroupManager();
	}

	return m_Instance;
}

void AiAgentGroupManager::Destroy()
{
	delete m_Instance;
	m_Instance = nullptr;
}

void AiAgentGroupManager::RegisterAIAgent(APawn* aiAgent)
{
	m_registeredAgents.AddUnique(aiAgent);
}

void AiAgentGroupManager::UnregisterAIAgent(APawn* aiAgent)
{
	m_registeredAgents.Remove(aiAgent);
}

void AiAgentGroupManager::instancierTableau(ACharacter* playerCharacter)
{
	if (!playerCharacter)
		return;

	TArray<AActor*> test;
	playerCharacter->GetAllChildActors(test);
	for (AActor* actor : test) {
		targetPositions.Add(Cast<ATargetPosition>(actor));
	}
	UE_LOG(LogTemp, Warning, TEXT("Taille targetPostion: %d"), targetPositions.Num());
}
