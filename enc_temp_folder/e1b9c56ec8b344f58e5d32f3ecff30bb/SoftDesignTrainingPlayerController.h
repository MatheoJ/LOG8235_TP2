// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "SDTPathFollowingComponent.h"
#include "SoftDesignTrainingPlayerController.generated.h"

class ASDTBridge;
class ASDTBoatOperator;

UCLASS()
class ASoftDesignTrainingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASoftDesignTrainingPlayerController();

    void BeginPlay() override;

protected:

    virtual void SetupInputComponent() override;

    void MoveCameraForward(float axisValue);
    void MoveCameraRight(float axisValue);
    void ZoomCamera(float axisValue);

    void MoveCharacter();

    void Activate();
    void Deactivate();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Navigation, meta = (AllowPrivateAccess = "true"))
    USDTPathFollowingComponent* m_PathFollowingComponent;

    bool m_CanMoveCharacter;

    ASDTBridge* m_BridgeActivated;
    ASDTBoatOperator* m_BoatOperatorActivated;
};


