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
	AiAgentGroupManager* groupManager = AiAgentGroupManager::GetInstance();
	//UE_LOG(LogTemp, Warning, TEXT("My group size is %d"), groupManager->m_registeredAgents.Num());
	if (m_ReachedTarget)
		{
			//DetectPlayer(deltaTime);
		}
		else {
			ShowNavigationPath();
		}
	//}

}


