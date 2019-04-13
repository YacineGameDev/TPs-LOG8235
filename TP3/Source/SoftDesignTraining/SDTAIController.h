// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDTBaseAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "SDTAIController.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API ASDTAIController : public ASDTBaseAIController
{
	GENERATED_BODY()

public:
    ASDTAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float m_DetectionCapsuleHalfLength = 500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float m_DetectionCapsuleRadius = 250.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float m_DetectionCapsuleForwardStartingOffset = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    UCurveFloat* JumpCurve;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float JumpApexHeight = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
    float JumpSpeed = 1.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    bool AtJumpSegment = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    bool InAir = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
    bool Landing = false;

	UBehaviorTreeComponent* GetBehaviorTreeComponent() const { return m_behaviorTreeComponent; }
	UBlackboardComponent* GetBlackBoardComponent() const { return m_blackboardComponent; }

	void StartBehaviorTree(APawn* pawn);
	void StopBehaviorTree(APawn* pawn);

	/*Methodes pour BehaviorTree*/

	uint8 GetPawnBBKeyID() const { return m_pawnBBKeyID; }
	uint8 GetTargetPosBBKeyID() const { return m_targetPosBBKeyID; }
	uint8 GetFleePosBBKeyID() const { return m_fleePosBBKeyID; }
	uint8 GetRandomCollectiblePosBBKeyID() const { return m_collectiblePosBBKeyID; }
	uint8 GetTargetSeenKeyID() const { return m_isTargetSeenBBKeyID; }
	uint8 GetTargetPoweredUpKeyID() const { return m_isTargetPoweredUpBBKeyID; }
	uint8 GetIsFleeingKeyID() const { return m_isFleeingBBKeyID; }

	int targetLocationIdx;

	float execTime_detectPlayer;
	float execTime_computeFleePos;
	float execTime_computePickUpPos;




protected:

	virtual void Possess(APawn* pawn) override;

public:
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
    void RotateTowards(const FVector& targetLocation);
    void SetActorLocation(const FVector& targetLocation);
    void AIStateInterrupted();
	void OnMoveToTarget();



private:
    virtual void ShowNavigationPath() override;
	virtual void DisplayExecTime() override;


	/*Attributs pour Behavior Tree*/

	UPROPERTY(transient)
	UBehaviorTreeComponent* m_behaviorTreeComponent;

	UPROPERTY(transient)
	UBlackboardComponent* m_blackboardComponent;

	uint8   m_targetPosBBKeyID;
	uint8   m_fleePosBBKeyID;
	uint8   m_collectiblePosBBKeyID;
	uint8   m_isTargetSeenBBKeyID;
	uint8   m_isTargetPoweredUpBBKeyID;
	uint8	m_pawnBBKeyID;
	uint8	m_isFleeingBBKeyID;


protected:
    FVector m_JumpTarget;
    FRotator m_ObstacleAvoidanceRotation;
    FTimerHandle m_PlayerInteractionNoLosTimer;

};
