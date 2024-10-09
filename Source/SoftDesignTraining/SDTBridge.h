// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SDTBridge.generated.h"

enum class EBridgeState
{
	BRIDGE_UP,
	BRIDGE_GOING_UP,
	BRIDGE_GOING_DOWN,
	BRIDGE_DOWN
};

UCLASS()
class SOFTDESIGNTRAINING_API ASDTBridge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASDTBridge();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Activate();
	void Deactivate();

	EBridgeState GetState() const;

protected:
	EBridgeState m_State;
	bool m_IsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	float m_BridgeOpeningAlpha;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
	float m_BridgeOpenningSpeed;
};
