// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "SDTPathFollowingComponent.generated.h"

/**
*
*/
UCLASS(ClassGroup = AI, config = Game)
class SOFTDESIGNTRAINING_API USDTPathFollowingComponent : public UPathFollowingComponent
{
    GENERATED_UCLASS_BODY()

public:
    virtual void FollowPathSegment(float deltaTime) override;
    virtual void SetMoveSegment(int32 segmentStartIndex) override;

    UPROPERTY(BlueprintReadOnly)
    float jumProgress{ 0.f };

    UPROPERTY(BlueprintReadOnly)
    bool isJumping{ false };

    UPROPERTY(BlueprintReadOnly)
    float currentSpeed;

private:
    FVector3d CalculateVelocity(const FVector& currentPos, const FVector& nextPos, float jumpduration);
    float jumpDuration{ 2.f };
};