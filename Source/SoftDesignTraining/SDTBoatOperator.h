// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDTBoatOperator.generated.h"

class ASDTBoatAIController;

UCLASS()
class SOFTDESIGNTRAINING_API ASDTBoatOperator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDTBoatOperator();

	virtual void Tick(float DeltaTime) override;

	void Activate();
	void Deactivate();

	void Reserve(ASDTBoatAIController* boat);
	bool IsAvailable() const;
	void ClearReservation();

	FVector GetDropLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* m_RootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* m_BoatDropPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boat")
	float m_TimeToEmptyContainer{ 5.f };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	ASDTBoatAIController* m_BoatController{ nullptr };

	float m_Accumulator{ 0.f };

	bool m_IsUnloading{ false };
};
