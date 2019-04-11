// Fill out your copyright notice in the Description page of Project Settings.

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

void AiAgentGroupManager::RegisterAIAgent(ASoftDesignTrainingCharacter* aiAgent)
{
	m_registeredAgents.Add(aiAgent);
}

void AiAgentGroupManager::UnregisterAIAgent(ASoftDesignTrainingCharacter* aiAgent)
{
	m_registeredAgents.Remove(aiAgent);
}
