// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTAIController.h"
#include "SoftDesignTraining.h"
#include "SDTBridge.h"
#include "SDTPathFollowingComponent.h"
#include "SoftDesignTrainingMainCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

#include "SDTUtils.h"
#include "EngineUtils.h"

#include "NavigationSystem.h"

ASDTAIController::ASDTAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<USDTPathFollowingComponent>(TEXT("PathFollowingComponent")))
{

}

void ASDTAIController::GoToBestTarget(float deltaTime)
{
    //Move to target depending on current behavior
    APawn* pawn = GetPawn();

    UPathFollowingComponent* path = GetPathFollowingComponent();
    if (path == nullptr)
    {
        return;
    }

    switch (m_PedestrianState)
    {
        case PedestrianState::SPAWNED:
        {
            FString tag("WaitPoint_Bridge_");
            AActor* actor = FindActorWithTag(tag);
            
            if (actor != nullptr)
            {
                // TODO : Agents wants to move towards actor

                m_ReachedTarget = false;
                ComputeAndFollowPathToTarget(actor->GetActorLocation());

                m_PedestrianState = PedestrianState::GO_TO_BRIDGE;
            }
            break;
        }
        case PedestrianState::GO_TO_BRIDGE:
        {
            // Nothing to do
            break;
        }
        case PedestrianState::WAIT_AT_BRIDGE:
        {
            // Check if bridge is down
            FString tag("Bridge_");
            AActor* actor = FindActorWithTag(tag);
            ASDTBridge* bridge = Cast<ASDTBridge>(actor);
            
            // Once the bridge is down, we go through
            if (bridge != nullptr && bridge->GetState() == EBridgeState::BRIDGE_DOWN)
            {
                m_PedestrianState = PedestrianState::GO_TO_DESPAWN;
            }

            break;
        }
        case PedestrianState::GO_TO_DESPAWN:
        {
            FString tag("Despawn_");
            AActor* actor = FindActorWithTag(tag);
            
            if (actor != nullptr)
            {
                // TODO : Agents wants to move towards actor

                m_ReachedTarget = false;
                ComputeAndFollowPathToTarget(actor->GetActorLocation());
            }
            
            break;
        }
        case PedestrianState::DESPAWN:
        {
            UnPossess();
            Destroy();

            pawn->Destroy();
            break;
        }
    }
}

void ASDTAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    Super::OnMoveCompleted(RequestID, Result);

    m_ReachedTarget = true;

    // Safeguard
    if (Result.Code == EPathFollowingResult::Aborted)
    {
        return;
    }

    switch (m_PedestrianState)
    {
        case PedestrianState::GO_TO_BRIDGE:
        {
            m_PedestrianState = PedestrianState::WAIT_AT_BRIDGE;
            break;
        }
        case PedestrianState::GO_TO_DESPAWN:
        {
            m_PedestrianState = PedestrianState::DESPAWN;
            break;
        }
    }
}

void ASDTAIController::ShowNavigationPath()
{
    // Show current navigation path DrawDebugLine and DrawDebugSphere
    // Use the UPathFollowingComponent of the AIController to get the path
    // This function is called while m_ReachedTarget is false 
    // Check void ASDTBaseAIController::Tick for how it works.

    //LOG here on the screen the path found
    GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, TEXT("INVALID DESTINATION USED1"));

    UPathFollowingComponent* path = GetPathFollowingComponent();
    if (path == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("INVALID DESTINATION USED1"));
		return;
	}

    FNavPathSharedPtr navPath = path->GetPath();

    if (navPath.IsValid())
    {
		TArray<FNavPathPoint> pathPoints = navPath->GetPathPoints();
        for (int i = 0; i < pathPoints.Num() - 1; i++)
        {
			DrawDebugLine(GetWorld(), pathPoints[i].Location, pathPoints[i + 1].Location, FColor::Green, false, 2.5f);
		}
    }
    else
    {
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("INVALID DESTINATION USED"));
	}
}

void ASDTAIController::AIStateInterrupted()
{
    StopMovement();

    m_ReachedTarget = false;
}

void ASDTAIController::ComputeAndFollowPathToTarget(const FVector& targetLocation)
{
	APawn* pawn = GetPawn();
    if (pawn == nullptr)
    {
		return;
	}

	UNavigationPath* path = UNavigationSystemV1::FindPathToLocationSynchronously(this, pawn->GetActorLocation(), targetLocation);
    if (path == nullptr)
    {
        return;
	}

    if (!path || !path->GetPath().IsValid() || path->GetPath()->IsPartial() || path->GetPath()->GetPathPoints().Num() == 0)
    {
        GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("INVALID DESTINATION USED"));
        return;
    }

    // Get the FNavPathSharedPtr
    FNavPathSharedPtr navPath = path->GetPath();

    UPathFollowingComponent* pathFollowComp = GetPathFollowingComponent();

    pathFollowComp->RequestMove(targetLocation, navPath);

}

bool ASDTAIController::IsPathStillValid()
{
	UPathFollowingComponent* path = GetPathFollowingComponent();
	if (path == nullptr)
	{
		return false;
	}

	FNavPathSharedPtr navPath = path->GetPath();

	// We don't want the AI to take the navlink.
	if (navPath.IsValid() && !navPath->IsPartial() && navPath->GetPathPoints().Num() > 0 && !navPath->ContainsAnyCustomLink())
	{
		return true;
	}

	return false;
}

void ASDTAIController::AbortPathFollowing()
{
	UPathFollowingComponent* path = GetPathFollowingComponent();
	if (path == nullptr)
	{
		return;
	}

	path->AbortMove(*this, FPathFollowingResultFlags::InvalidPath);
	// Reset the AI state. The AI will go back in front of the bridge.
	m_PedestrianState = PedestrianState::SPAWNED;
}