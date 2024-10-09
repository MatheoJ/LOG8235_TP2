// Fill out your copyright notice in the Description page of Project Settings.


#include "SDTBoat.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ASDTBoat::ASDTBoat()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASDTBoat::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASDTBoat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASDTBoat::UnloadContainer(float Amount)
{
	m_Container = FMath::Clamp(m_Container - Amount, 0.f, 1.f);
}

float ASDTBoat::GetContainerAmount() const
{
	return m_Container;
}