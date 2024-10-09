// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDTAISpawner.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API ASDTAISpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDTAISpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Spawn();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	float m_CooldownToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawner")
	float m_CurrentCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	UClass* m_AIClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	FString m_TagToLookFor;
};
