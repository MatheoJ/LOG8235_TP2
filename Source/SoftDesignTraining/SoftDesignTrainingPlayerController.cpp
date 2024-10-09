// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "SoftDesignTrainingPlayerController.h"
#include "SoftDesignTraining.h"
#include "SoftDesignTrainingMainCharacter.h"

#include "DrawDebugHelpers.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "SDTBridge.h"
#include "SDTBoatOperator.h"

#include "NavigationSystem.h"
#include <NavigationPath.h>

ASoftDesignTrainingPlayerController::ASoftDesignTrainingPlayerController()
{
    // Make a path following component
    m_PathFollowingComponent = CreateDefaultSubobject<USDTPathFollowingComponent>(TEXT("PathFollowingComponent"));
}

void ASoftDesignTrainingPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Move camera
    InputComponent->BindAxis("MoveForward", this, &ASoftDesignTrainingPlayerController::MoveCameraForward);
    InputComponent->BindAxis("MoveRight", this, &ASoftDesignTrainingPlayerController::MoveCameraRight);

    // Zoom camera
    InputComponent->BindAxis("Zoom", this, &ASoftDesignTrainingPlayerController::ZoomCamera);

    // Move character on click
    InputComponent->BindAction("MoveCharacter", EInputEvent::IE_Released, this, &ASoftDesignTrainingPlayerController::MoveCharacter);
    
    InputComponent->BindAction("Activate", EInputEvent::IE_Pressed, this, &ASoftDesignTrainingPlayerController::Activate);
    InputComponent->BindAction("Activate", EInputEvent::IE_Released, this, &ASoftDesignTrainingPlayerController::Deactivate);
}

void ASoftDesignTrainingPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Always show the mouse
    bShowMouseCursor = true;

    // Allow character to be moved by default 
    m_CanMoveCharacter = true;

    // In case we are activating a bridge
    m_BridgeActivated = nullptr;

    // In case we are activating a boat operator
    m_BoatOperatorActivated = nullptr;
}

void ASoftDesignTrainingPlayerController::MoveCameraForward(float value)
{
    ASoftDesignTrainingMainCharacter* character = Cast<ASoftDesignTrainingMainCharacter>(GetPawn());

    if (character)
    {
        character->MoveCameraForward(value);
    }
}

void ASoftDesignTrainingPlayerController::MoveCameraRight(float value)
{
    ASoftDesignTrainingMainCharacter* character = Cast<ASoftDesignTrainingMainCharacter>(GetPawn());

    if (character)
    {
        character->MoveCameraRight(value);
    }
}

void ASoftDesignTrainingPlayerController::ZoomCamera(float axisValue)
{
    ASoftDesignTrainingMainCharacter* character = Cast<ASoftDesignTrainingMainCharacter>(GetPawn());

    if (character)
    {
        character->ZoomCamera(axisValue);
    }
}

void ASoftDesignTrainingPlayerController::MoveCharacter()
{
    // TODO : find the position of the mouse in the world 
    // And move the agent to this position IF possible
    // Validate you can move through m_CanMoveCharacter

    if (!m_CanMoveCharacter)
    {
		return;
	}

    FVector2d mousePosition;
    GetMousePosition(mousePosition.X, mousePosition.Y);

    FHitResult hitResult;
    bool hit = GetHitResultAtScreenPosition(mousePosition, ECollisionChannel::ECC_WorldStatic, false, hitResult);

    //Log on screen the position of the mouse
    FString mousePositionString = "Mouse Position : " + mousePosition.ToString();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, mousePositionString);
    //Draw a debug sphere at the position of the mouse in the world
    DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 25.f, 12, FColor::Blue, false, 2.5f);

    if (hit)
    {
		FVector destination = hitResult.ImpactPoint;
		APawn* pawn = GetPawn();
        if (pawn)
        {
            //UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, destination);

            UNavigationPath* path = UNavigationSystemV1::FindPathToLocationSynchronously(this, pawn->GetActorLocation(), destination);
            if (!path || !path->GetPath().IsValid() || path->GetPath()->IsPartial() || path->GetPath()->GetPathPoints().Num() == 0)
            {
                GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("INVALID DESTINATION USED"));
                return;
            }

            // Get the FNavPathSharedPtr
            FNavPathSharedPtr navPath = path->GetPath();

            
            m_PathFollowingComponent->RequestMove(destination, navPath);

            // Draw the path
            if (navPath.IsValid())
            {
				TArray<FNavPathPoint> pathPoints = navPath->GetPathPoints();
                for (int i = 0; i < pathPoints.Num() - 1; i++)
                {
					DrawDebugLine(GetWorld(), pathPoints[i].Location, pathPoints[i + 1].Location, FColor::Green, false, 2.5f);
				}
                //Log on screen the position of the mouse
                FString message = "Path found";
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, message);
            }
			
		}
	}



}

void ASoftDesignTrainingPlayerController::Activate()
{
    APawn* pawn = GetPawn();
    if (pawn == nullptr)
    {
        return;
    }

    m_CanMoveCharacter = false;
    // TODO : Mouvement of the agent should be stopped !!

    m_PathFollowingComponent->AbortMove(*this, FPathFollowingResultFlags::MovementStop);

    // Make an overlap to find what is near us to activate it
    TArray<FOverlapResult> results;
    GetWorld()->OverlapMultiByChannel(results, pawn->GetActorLocation(), pawn->GetActorRotation().Quaternion(), ECollisionChannel::ECC_WorldDynamic, FCollisionShape::MakeSphere(200.f));

    for (FOverlapResult& result : results)
    {
        AActor* actor = result.GetActor();
        ASDTBridge* bridge = Cast<ASDTBridge>(actor);
        if (bridge)
        {
            bridge->Activate();
            m_BridgeActivated = bridge;
            break;
        }

        ASDTBoatOperator* boatOperator = Cast<ASDTBoatOperator>(actor);
        if (boatOperator)
        {
            boatOperator->Activate();
            m_BoatOperatorActivated = boatOperator;
            break;
        }
    }
}

void ASoftDesignTrainingPlayerController::Deactivate()
{
    m_CanMoveCharacter = true;

    if (m_BridgeActivated)
    {
        m_BridgeActivated->Deactivate();
    }
    if (m_BoatOperatorActivated)
    {
        m_BoatOperatorActivated->Deactivate();
    }
}