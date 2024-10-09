// Fill out your copyright notice in the Description page of Project Settings.


#include "SDTAISpawner.h"
#include "SDTBaseAIController.h"

// Sets default values
ASDTAISpawner::ASDTAISpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CooldownToSpawn = 60.f;
	m_CurrentCooldown = 0.f;
}

// Called when the game starts or when spawned
void ASDTAISpawner::BeginPlay()
{
	Super::BeginPlay();
	
	Spawn();
}

// Called every frame
void ASDTAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_CurrentCooldown += DeltaTime;
	if (m_CurrentCooldown >= m_CooldownToSpawn)
	{
		m_CurrentCooldown = 0.f;
		Spawn();
	}
}

void ASDTAISpawner::Spawn()
{
	if (m_AIClassToSpawn != nullptr)
	{
		FActorSpawnParameters parameters;
		parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		APawn* npc = GetWorld()->SpawnActor<APawn>(m_AIClassToSpawn, GetActorLocation(), GetActorRotation(), parameters);
		if (npc)
		{
			npc->SpawnDefaultController();

			ASDTBaseAIController* controller = Cast<ASDTBaseAIController>(npc->GetController());
			if (controller != nullptr)
			{
				controller->SetTagToLookFor(m_TagToLookFor);
			}
		}
	}

	m_CurrentCooldown = 0.f;
}

