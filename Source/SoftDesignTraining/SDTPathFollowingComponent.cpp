// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTPathFollowingComponent.h"
#include "SoftDesignTraining.h"
#include "SDTUtils.h"
#include "SDTAIController.h"
#include "SoftDesignTrainingPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "NavLinkCustomInterface.h"

#include "DrawDebugHelpers.h"

USDTPathFollowingComponent::USDTPathFollowingComponent(const FObjectInitializer& ObjectInitializer)
{

}

/**
* This function is called every frame while the AI is following a path.
* MoveSegmentStartIndex and MoveSegmentEndIndex specify where we are on the path point array.
*/
void USDTPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
    const TArray<FNavPathPoint>& points = Path->GetPathPoints();
    const FNavPathPoint& segmentStart = points[MoveSegmentStartIndex];
    const FNavPathPoint& segmentEnd = points[MoveSegmentEndIndex];

    if (SDTUtils::HasJumpFlag(segmentStart))
    {
        // Update jump along path / nav link proxy

    }
    else
    {
        // Update normal movement
		Super::FollowPathSegment(DeltaTime);
    }
}

/**
* This function is called every time the AI has reached a new point on the path.
* If you need to do something at a given point in the path, this is the place.
*/
void USDTPathFollowingComponent::SetMoveSegment(int32 segmentStartIndex)
{
    Super::SetMoveSegment(segmentStartIndex);

    const TArray<FNavPathPoint>& points = Path->GetPathPoints();

    const FNavPathPoint& segmentStart = points[MoveSegmentStartIndex];

    UCharacterMovementComponent* characterMovementComponent;

    ASDTAIController* aIController = Cast<ASDTAIController>(GetOwner());   
    
    if (aIController != nullptr)
    {
        characterMovementComponent = aIController->GetCharacter()->GetCharacterMovement();
	}
    else
    {
        ASoftDesignTrainingPlayerController *playerController = Cast<ASoftDesignTrainingPlayerController>(GetOwner());
        characterMovementComponent = playerController->GetCharacter()->GetCharacterMovement();
	}

    

    if (SDTUtils::HasJumpFlag(segmentStart) && FNavMeshNodeFlags(segmentStart.Flags).IsNavLink())
    {
        isJumping = true;

        characterMovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);

        //Calculate velocity
        FVector velocity = CalculateVelocity(segmentStart.Location, points[MoveSegmentStartIndex + 1].Location, 2.0f);
        characterMovementComponent->Launch(FVector(velocity.X, velocity.Y, FMath::Abs(velocity.Z)));

    }
    else
    {
        isJumping= false;
        characterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
    }
}

FVector USDTPathFollowingComponent::CalculateVelocity(const FVector& currentPos, const FVector& nextPos, float jumpduration)
{
    FVector velocity = FVector::ZeroVector;

    double x = nextPos.X - currentPos.X;
    double y = nextPos.Y - currentPos.Y;

    x = x / jumpduration;
    y = y / jumpduration;

    double z = nextPos.Z - currentPos.Z + (jumpduration * jumpduration * -0.5 * 980);
    z = z / jumpduration;

    velocity.X = x;
    velocity.Y = y;
    velocity.Z = z;

    return velocity;
}

