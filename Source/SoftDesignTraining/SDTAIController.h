// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDTBaseAIController.h"
#include "SDTAIController.generated.h"

enum class ActionPriority
{
    FLEE,
    PLAYER,
    LKP,
    COLLECTIBLE,
};

enum class PedestrianState
{
    SPAWNED,
    GO_TO_BRIDGE,
    WAIT_AT_BRIDGE,
    GO_TO_DESPAWN,
    DESPAWN
};

/**
 * 
 */
UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API ASDTAIController : public ASDTBaseAIController
{
	GENERATED_BODY()

public:
    ASDTAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
    void AIStateInterrupted();


private:
    virtual void GoToBestTarget(float deltaTime) override;

    void ComputeAndFollowPathToTarget(const FVector& targetLocation);   

    virtual void ShowNavigationPath() override;

    PedestrianState m_PedestrianState{ PedestrianState::SPAWNED };

};
