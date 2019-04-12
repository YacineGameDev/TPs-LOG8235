// Fill out your copyright notice in the Description page of Project Settings.
#include "SoftDesignTraining.h"
#include "AiAgentGroupManager.h"
#include "SDTAIController.h"
#include "SDTUtils.h"

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
	if (ASDTAIController* aiController = Cast<ASDTAIController>(aiAgent->GetController()))
	{
		FreeLocation(aiController->targetLocationIdx);
		aiController->targetLocationIdx = -1;
	}

}

void AiAgentGroupManager::initTargetPos(ACharacter* playerCharacter)
{
	if (!playerCharacter)
		return;

	TArray<AActor*> test;
	playerCharacter->GetAllChildActors(test);
	for (AActor* actor : test) {
		targetPositions.Add(Cast<ATargetPosition>(actor));
	}
	// UE_LOG(LogTemp, Warning, TEXT("Taille targetPostion: %d"), targetPositions.Num());
}

FVector AiAgentGroupManager::allocateTargetPos(ACharacter* character, int & idx)
{
	FVector characterLocation = character->GetActorLocation();
	float closestDistance = TNumericLimits< float >::Max();
	FVector bestTargetLocation = FVector::ZeroVector;

	for (ATargetPosition* target : targetPositions) {
		if (target->isFree)
		{
			float distanceTargetPlayer = (characterLocation - target->GetActorLocation()).Size();
			if (distanceTargetPlayer < closestDistance)
			{
				/*FPathFindingQuery query = FPathFindingQuery(Cast<UObject>(character), *character->GetWorld()->GetNavigationSystem()->GetMainNavData(), characterLocation, target->GetActorLocation());
				bool isReachable = character->GetWorld()->GetNavigationSystem()->TestPathSync(query);
				if (isReachable)
				{
				*/
					targetPositions.Find(target, idx);
					bestTargetLocation = target->GetActorLocation();
					closestDistance = distanceTargetPlayer;
					target->isFree = false;
				//}	
			}
		}		
	}

	if (bestTargetLocation == FVector::ZeroVector)
	{
		int randIdx = rand() % targetPositions.Num();
		ATargetPosition* target = targetPositions[randIdx];
		//FPathFindingQuery query = FPathFindingQuery(Cast<UObject>(character), *character->GetWorld()->GetNavigationSystem()->GetMainNavData(), characterLocation, target->GetActorLocation());
		/*bool isReachable = character->GetWorld()->GetNavigationSystem()->TestPathSync(query);
		if (isReachable)
		{*/
			bestTargetLocation = target->GetActorLocation();
			idx = randIdx;
	//	}
	}

	return bestTargetLocation;
}

void AiAgentGroupManager::FreeLocation(int idx)
{
	if (idx != -1) {
		targetPositions[idx]->isFree = true;
	}
}

