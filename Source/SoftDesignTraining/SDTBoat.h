// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SDTBoat.generated.h"

UCLASS()
class SOFTDESIGNTRAINING_API ASDTBoat : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASDTBoat();

	void UnloadContainer(float Amount);
	float GetContainerAmount() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boat")
	float m_Container{ 1.f };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float m_Accumulator{ 0.f };
};
