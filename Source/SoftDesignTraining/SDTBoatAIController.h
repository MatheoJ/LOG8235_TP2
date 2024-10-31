// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SDTBaseAIController.h"
#include "SDTBoatAIController.generated.h"

enum class BoatState
{
    SPAWNED,
    GO_TO_START_BRIDGE,
    WAIT_AT_START_BRIDGE,
    GO_TO_OPERATOR,
    WAIT_AT_OPERATOR,
    GO_TO_END_BRIDGE,
    WAIT_AT_END_BRIDGE,
    GO_TO_DESPAWN,
    DESPAWN
};

/**
 *
 */
UCLASS()
class SOFTDESIGNTRAINING_API ASDTBoatAIController : public ASDTBaseAIController
{
    GENERATED_BODY()
public:
    virtual void Tick(float deltaTime) override;

    void NotifyUnloadComplete();

    virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
    void AIStateInterrupted();

    BoatState GetBoatState();
    void ComputeAndFollowPathToTarget(const FVector& targetLocation);

private:
    virtual void GoToBestTarget(float deltaTime) override;
    virtual void ShowNavigationPath() override;
    FVector CurrentTargetLocation = FVector::ZeroVector;
    bool reserved = false;

    BoatState m_BoatState{ BoatState::SPAWNED };
};
