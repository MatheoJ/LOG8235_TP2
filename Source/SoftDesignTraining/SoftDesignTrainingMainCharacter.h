// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoftDesignTrainingCharacter.h"
#include "SoftDesignTrainingMainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API ASoftDesignTrainingMainCharacter : public ASoftDesignTrainingCharacter
{
    GENERATED_BODY()
public:
    ASoftDesignTrainingMainCharacter();

    void MoveCameraForward(float value);
    void MoveCameraRight(float value);
    void ZoomCamera(float value);

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* m_TopDownCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* m_CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    float m_CameraSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    FVector2D m_MaxCameraBounds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    FVector2D m_MinCameraBounds;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    float m_MinZoom;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    float m_MaxZoom;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    float m_ZoomSpeed;
};
