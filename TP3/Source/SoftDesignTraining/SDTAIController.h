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

	FVector GetTargetPlayerPos() const { return m_targetPlayerPos; }
	FVector GetFleePos() const { return m_fleePos; }
	FVector GetRandomCollectiblePos() const { return m_collectiblePos; }
	bool IsTargetPlayerSeen() const { return m_isPlayerDetected; }
	bool IsPlayerPoweredUp() const { return m_isPlayerPoweredUp; }

	uint8 GetPawnBBKeyID() const { return m_pawnBBKeyID; }
	uint8 GetTargetPosBBKeyID() const { return m_targetPosBBKeyID; }
	uint8 GetFleePosBBKeyID() const { return m_fleePosBBKeyID; }
	uint8 GetRandomCollectiblePosBBKeyID() const { return m_collectiblePosBBKeyID; }
	uint8 GetTargetSeenKeyID() const { return m_isTargetSeenBBKeyID; }
	uint8 GetTargetPoweredUpKeyID() const { return m_isTargetPoweredUpBBKeyID; }







	void MovePawn(FVector targetLocation);

	/******/
	//TOM STUFF
	enum PlayerInteractionBehavior
	{
		PlayerInteractionBehavior_Collect,
		PlayerInteractionBehavior_Chase,
		PlayerInteractionBehavior_Flee
	};



protected:

    /*enum PlayerInteractionBehavior
    {
        PlayerInteractionBehavior_Collect,
        PlayerInteractionBehavior_Chase,
        PlayerInteractionBehavior_Flee
    };*/

    void GetHightestPriorityDetectionHit(const TArray<FHitResult>& hits, FHitResult& outDetectionHit);
    void UpdatePlayerInteractionBehavior(const FHitResult& detectionHit, float deltaTime);
    PlayerInteractionBehavior GetCurrentPlayerInteractionBehavior(const FHitResult& hit);
    bool HasLoSOnHit(const FHitResult& hit);
    void MoveToRandomCollectible();
    void MoveToBestFleeLocation();
    void PlayerInteractionLoSUpdate();
    void OnPlayerInteractionNoLosDone();


	virtual void Possess(APawn* pawn) override;

public:
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
    void RotateTowards(const FVector& targetLocation);
    void SetActorLocation(const FVector& targetLocation);
    void AIStateInterrupted();
	FVector GetBestFleeLocation();
	FVector GetRandomCollectibleLocation();
	void OnMoveToTarget();
	void MoveToPlayer();
	PlayerInteractionBehavior GetPlayerInteractionBehavior() const { return m_PlayerInteractionBehavior; }
	virtual void UpdatePlayerInteraction(float deltaTime) override;



private:
    //virtual void GoToBestTarget(float deltaTime) override;
	//virtual void DetectPlayer(float deltaTime) override;
    virtual void ShowNavigationPath() override;



	/*Attributs pour Behavior Tree*/

	UPROPERTY(transient)
	UBehaviorTreeComponent* m_behaviorTreeComponent;

	UPROPERTY(transient)
	UBlackboardComponent* m_blackboardComponent;

	FVector m_targetPlayerPos;
	FVector m_fleePos;
	FVector m_collectiblePos;
	bool m_isPlayerDetected;
	bool m_isPlayerPoweredUp;

	uint8   m_targetPosBBKeyID;
	uint8   m_fleePosBBKeyID;
	uint8   m_collectiblePosBBKeyID;
	uint8   m_isTargetSeenBBKeyID;
	uint8   m_isTargetPoweredUpBBKeyID;
	uint8	m_pawnBBKeyID;

	/*******/

protected:
    FVector m_JumpTarget;
    FRotator m_ObstacleAvoidanceRotation;
    FTimerHandle m_PlayerInteractionNoLosTimer;
    PlayerInteractionBehavior m_PlayerInteractionBehavior;

};
