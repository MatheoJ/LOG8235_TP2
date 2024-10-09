// Fill out your copyright notice in the Description page of Project Settings.

#include "SDTBoatOperator.h"
#include "SDTBoatAIController.h"
#include "SDTBoat.h"

// Sets default values
ASDTBoatOperator::ASDTBoatOperator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	RootComponent = m_RootComponent;

	m_BoatDropPoint = CreateDefaultSubobject<USceneComponent>("DropPoint");
	m_BoatDropPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASDTBoatOperator::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASDTBoatOperator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_BoatController == nullptr || !m_IsUnloading)
	{
		return;
	}

	ASDTBoat* boat = Cast<ASDTBoat>(m_BoatController->GetPawn());

	if (boat == nullptr || m_BoatController->GetBoatState() != BoatState::WAIT_AT_OPERATOR)
	{
		return;
	}

	float amount = DeltaTime / m_TimeToEmptyContainer;
	boat->UnloadContainer(amount); 

	float containerAmount = boat->GetContainerAmount();
	if (FMath::IsNearlyZero(containerAmount))
	{
		m_BoatController->NotifyUnloadComplete();
		ClearReservation();
	}
}

void ASDTBoatOperator::Activate()
{
	m_IsUnloading = true;
}

void ASDTBoatOperator::Deactivate()
{
	m_IsUnloading = false;
}

void ASDTBoatOperator::Reserve(ASDTBoatAIController* boatController)
{
	if (!IsAvailable())
	{
		return;
	}

	m_BoatController = boatController;
}

bool ASDTBoatOperator::IsAvailable() const
{
	return m_BoatController == nullptr;
}

void ASDTBoatOperator::ClearReservation()
{
	m_BoatController = nullptr;
}

FVector ASDTBoatOperator::GetDropLocation()
{
	return m_BoatDropPoint->GetComponentLocation();
}