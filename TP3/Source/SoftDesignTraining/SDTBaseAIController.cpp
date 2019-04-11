// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTBaseAIController.h"


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

	//if (GetPawn()->GetName() == "BP_SDTAICharacter46") {

		// UE_LOG(LogTemp, Warning, TEXT("m_ReachedTarget: %s"), (m_ReachedTarget ? TEXT("True*******************************************************************") : TEXT("False")));
		//UpdatePlayerInteraction(deltaTime);

		if (m_ReachedTarget)
		{
			//DetectPlayer(deltaTime);
		}
		else {
			ShowNavigationPath();
		}
	//}

}


