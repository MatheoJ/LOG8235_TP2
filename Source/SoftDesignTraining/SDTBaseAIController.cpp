// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTBaseAIController.h"
#include "SoftDesignTraining.h"
#include "SDTPathFollowingComponent.h"

ASDTBaseAIController::ASDTBaseAIController(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
    m_ReachedTarget = true;
}

void ASDTBaseAIController::Tick(float deltaTime)
{
    Super::Tick(deltaTime);

    if (m_ReachedTarget)
    {
        GoToBestTarget(deltaTime);
    }
    else
    {
        if (IsPathStillValid()) {
            ShowNavigationPath();
        }
        else {
			AbortPathFollowing();
        }
    }
}

AActor* ASDTBaseAIController::FindActorWithTag(FString actorTag, bool appendTag)
{
    if (appendTag)
    {
        actorTag.Append(m_TagToLookFor);
    }   

    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsWithTag(this, *actorTag, foundActors);

    if (foundActors.IsEmpty())
    {
        return nullptr;
    }
    {
        return foundActors[0];
    }
}

void ASDTBaseAIController::SetTagToLookFor(const FString& TagToLookFor)
{
    m_TagToLookFor = TagToLookFor;
}

