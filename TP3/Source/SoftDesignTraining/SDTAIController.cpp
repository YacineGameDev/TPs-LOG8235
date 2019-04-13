// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTraining.h"
#include "SDTAIController.h"
#include "SDTCollectible.h"
#include "SDTFleeLocation.h"
#include "SDTPathFollowingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "UnrealMathUtility.h"
#include "SDTUtils.h"
#include "SoftDesignTrainingCharacter.h"
#include "EngineUtils.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"



ASDTAIController::ASDTAIController(const FObjectInitializer& ObjectInitializer)
    : m_isTargetSeenBBKeyID(0)
	,m_isTargetPoweredUpBBKeyID(0)
	,m_fleePosBBKeyID(0)
	,m_targetPosBBKeyID(0)
	,targetLocationIdx(-1)
	,execTime_detectPlayer(0.f)
	,execTime_computeFleePos(0.f)
	,execTime_computePickUpPos(0.f)
	,Super(ObjectInitializer.SetDefaultSubobjectClass<USDTPathFollowingComponent>(TEXT("PathFollowingComponent")))
{
	m_behaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	m_blackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ASDTAIController::StartBehaviorTree(APawn* pawn) {
	if (ASoftDesignTrainingCharacter* character = Cast<ASoftDesignTrainingCharacter>(pawn))
	{
		if (character->GetBehaviorTree())
		{
			m_behaviorTreeComponent->StartTree(*character->GetBehaviorTree());
		}
	}
}

void ASDTAIController::StopBehaviorTree(APawn* pawn) {
	if (ASoftDesignTrainingCharacter* character = Cast<ASoftDesignTrainingCharacter>(pawn))
	{
		if (character->GetBehaviorTree())
		{
			m_behaviorTreeComponent->StopTree();
		}
	}
}

void ASDTAIController::OnMoveToTarget()
{
    m_ReachedTarget = false;
}

void ASDTAIController::RotateTowards(const FVector& targetLocation)
{
    if (!targetLocation.IsZero())
    {
        FVector direction = targetLocation - GetPawn()->GetActorLocation();
        FRotator targetRotation = direction.Rotation();

        targetRotation.Yaw = FRotator::ClampAxis(targetRotation.Yaw);

        SetControlRotation(targetRotation);
    }
}

void ASDTAIController::SetActorLocation(const FVector& targetLocation)
{
    GetPawn()->SetActorLocation(targetLocation);
}

void ASDTAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    m_ReachedTarget = true;
}

void ASDTAIController::ShowNavigationPath()
{
    if (UPathFollowingComponent* pathFollowingComponent = GetPathFollowingComponent())
    {
        if (pathFollowingComponent->HasValidPath())
        {
            const FNavPathSharedPtr path = pathFollowingComponent->GetPath();
            TArray<FNavPathPoint> pathPoints = path->GetPathPoints();

            for (int i = 0; i < pathPoints.Num(); ++i)
            {
                DrawDebugSphere(GetWorld(), pathPoints[i].Location, 10.f, 8, FColor::Yellow);

                if (i != 0)
                {
                    DrawDebugLine(GetWorld(), pathPoints[i].Location, pathPoints[i - 1].Location, FColor::Yellow);
                }
            }
        }
    }
}

void ASDTAIController::DisplayExecTime() {
	DrawDebugString(GetWorld(), FVector(100.f, 0.f, 50.f), "DETECT: " + FString::SanitizeFloat(execTime_detectPlayer), GetPawn(), FColor::Orange, 0.f, false);
	DrawDebugString(GetWorld(), FVector(-100.f, 0.f, 50.f), "FLEE: " + FString::SanitizeFloat(execTime_computeFleePos), GetPawn(), FColor::Orange, 0.f, false);
	DrawDebugString(GetWorld(), FVector(0.f, 100, 50.f), "PICKUP: " + FString::SanitizeFloat(execTime_computePickUpPos), GetPawn(), FColor::Orange, 0.f, false);

}

void ASDTAIController::AIStateInterrupted()
{
    StopMovement();
    m_ReachedTarget = true;
}


void ASDTAIController::Possess(APawn* pawn)
{

	Super::Possess(pawn);

	if (ASoftDesignTrainingCharacter* character = Cast<ASoftDesignTrainingCharacter>(pawn))
	{
		if (character->GetBehaviorTree())
		{
			m_blackboardComponent->InitializeBlackboard(*character->GetBehaviorTree()->BlackboardAsset);

			m_targetPosBBKeyID = m_blackboardComponent->GetKeyID("TargetPos");
			m_isTargetSeenBBKeyID = m_blackboardComponent->GetKeyID("TargetIsSeen");
			m_isTargetPoweredUpBBKeyID = m_blackboardComponent->GetKeyID("IsPlayerPoweredUp");
			m_fleePosBBKeyID = m_blackboardComponent->GetKeyID("FleePos");
			m_collectiblePosBBKeyID = m_blackboardComponent->GetKeyID("CollectiblePos");
			m_pawnBBKeyID = m_blackboardComponent->GetKeyID("SelfActor");
			m_isFleeingBBKeyID = m_blackboardComponent->GetKeyID("isFleeing");

			//Set this agent in the BT
			m_blackboardComponent->SetValue<UBlackboardKeyType_Object>(m_blackboardComponent->GetKeyID("SelfActor"), pawn);
			m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(m_blackboardComponent->GetKeyID("TargetPos"), FVector::ZeroVector);
			m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(m_blackboardComponent->GetKeyID("FleePos"), FVector::ZeroVector);
			m_blackboardComponent->SetValue<UBlackboardKeyType_Vector>(m_blackboardComponent->GetKeyID("CollectiblePos"), FVector::ZeroVector);
			m_blackboardComponent->SetValue<UBlackboardKeyType_Bool>(m_blackboardComponent->GetKeyID("IsPlayerPoweredUp"), false);
			m_blackboardComponent->SetValue<UBlackboardKeyType_Bool>(m_blackboardComponent->GetKeyID("TargetIsSeen"), false);
			m_blackboardComponent->SetValue<UBlackboardKeyType_Bool>(m_blackboardComponent->GetKeyID("isFleeing"), false);

		}
	}
}