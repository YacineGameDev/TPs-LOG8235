// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTBaseAIController.h"
#include "AiAgentGroupManager.h"


ASDTBaseAIController::ASDTBaseAIController(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
    m_ReachedTarget = true;
}

void ASDTBaseAIController::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

	//TO THINK: QUAND AGENT MEUR REINITIALISER VALEUR BLACKBOARD
	// AGENT PROBLEME AVEC DALLE BP_SDTDeathFloor42

	DisplayExecTime();

	if (m_ReachedTarget)
	{
		// update
	}
	else {
		ShowNavigationPath();
	}
}


