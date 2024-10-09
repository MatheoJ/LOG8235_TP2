// Fill out your copyright notice in the Description page of Project Settings.

#include "SoftDesignTrainingMainCharacter.h"
#include "SoftDesignTraining.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASoftDesignTrainingMainCharacter::ASoftDesignTrainingMainCharacter()
{
    // Create a camera boom...
    m_CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    m_CameraBoom->SetupAttachment(RootComponent);
    m_CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
    m_CameraBoom->TargetArmLength = 400.f;
    m_CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
    m_CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

                                            // Create a camera...
    m_TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    m_TopDownCameraComponent->SetupAttachment(m_CameraBoom, USpringArmComponent::SocketName);
    m_TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm;

    // Set camera properties 
    m_CameraSpeed = 50.f;

    m_MaxCameraBounds = FVector2D(3000, 4000);
    m_MinCameraBounds = FVector2D(-4000, -4000);

    m_MinZoom = 200.f;
    m_MaxZoom = 2000.f;

    m_ZoomSpeed = 20.f;
}

void ASoftDesignTrainingMainCharacter::MoveCameraForward(float value)
{
    // No camera movement.
}

void ASoftDesignTrainingMainCharacter::MoveCameraRight(float value)
{
    // No camera movement.
}

void ASoftDesignTrainingMainCharacter::ZoomCamera(float value)
{
    if (FMath::IsNearlyZero(value))
    {
        return;
    }

    m_CameraBoom->TargetArmLength = FMath::Clamp(m_CameraBoom->TargetArmLength - value * m_ZoomSpeed, m_MinZoom, m_MaxZoom);
}