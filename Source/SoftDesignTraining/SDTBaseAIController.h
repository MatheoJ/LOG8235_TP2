// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SDTBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class SOFTDESIGNTRAINING_API ASDTBaseAIController : public AAIController
{
	GENERATED_BODY()

public:

    ASDTBaseAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
    virtual void Tick(float deltaTime) override;

    void SetTagToLookFor(const FString& TagToLookFor);
	
protected:
    AActor* FindActorWithTag(FString actorTag, bool appendTag = true);

    bool m_ReachedTarget;
    FString m_TagToLookFor;

private:
    virtual void GoToBestTarget(float deltaTime) {};
    virtual void ChooseBehavior(float deltaTime) {};
    virtual void ShowNavigationPath() {};

	virtual bool IsPathStillValid() { return false; };
	virtual void AbortPathFollowing() {};
};
