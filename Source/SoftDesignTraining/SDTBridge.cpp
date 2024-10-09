// Fill out your copyright notice in the Description page of Project Settings.


#include "SDTBridge.h"

// Sets default values
ASDTBridge::ASDTBridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASDTBridge::BeginPlay()
{
	Super::BeginPlay();
	
	m_State = EBridgeState::BRIDGE_UP;
	m_IsMoving = false;
	m_BridgeOpeningAlpha = 1.0f;
	m_BridgeOpenningSpeed = 0.5f;
}

// Called every frame
void ASDTBridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_IsMoving)
	{
		return;
	}

	if (m_State == EBridgeState::BRIDGE_GOING_UP)
	{
		m_BridgeOpeningAlpha += DeltaTime * m_BridgeOpenningSpeed;
		if (m_BridgeOpeningAlpha > 1.f)
		{
			m_BridgeOpeningAlpha = 1.f;
			m_IsMoving = false;
			m_State = EBridgeState::BRIDGE_UP;
		}
	}
	else if (m_State == EBridgeState::BRIDGE_GOING_DOWN)
	{
		m_BridgeOpeningAlpha -= DeltaTime * m_BridgeOpenningSpeed;
		if (m_BridgeOpeningAlpha < 0.f)
		{
			m_BridgeOpeningAlpha = 0.f;
			m_IsMoving = false;
			m_State = EBridgeState::BRIDGE_DOWN;
		}
	}
}

void ASDTBridge::Activate()
{
	if (m_State == EBridgeState::BRIDGE_UP)
	{
		m_State = EBridgeState::BRIDGE_GOING_DOWN;
	}
	else if (m_State == EBridgeState::BRIDGE_DOWN)
	{
		m_State = EBridgeState::BRIDGE_GOING_UP;
	}

	m_IsMoving = true;
}

void ASDTBridge::Deactivate()
{
	m_IsMoving = false;
}

EBridgeState ASDTBridge::GetState() const
{
	return m_State;
}