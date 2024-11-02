// Fill out your copyright notice in the Description page of Project Settings.
// Note: Activer enabledreawing dans recastNavmesh-boat sur unreal pour activer le second Navmesh


#include "SDTBoatAIController.h"
#include "Kismet/GameplayStatics.h"
#include "SDTPathFollowingComponent.h"
#include "SDTBridge.h"
#include "SDTBoat.h"

#include "SDTBoatOperator.h"
#include <NavigationSystem.h>
#include "SDTUtils.h"

void ASDTBoatAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	//this->GoToBestTarget(deltaTime);
}

void ASDTBoatAIController::ComputeAndFollowPathToTarget(const FVector& targetLocation)
{

	ASDTBoat* boat = Cast<ASDTBoat>(GetPawn());
	if (boat == nullptr)
	{
		return;
	}

		 MoveToLocation(targetLocation);




}

void ASDTBoatAIController::GoToBestTarget(float deltaTime)
{
	APawn* pawn = GetPawn();
	ASDTBoat* boat = Cast<ASDTBoat>(pawn);


	switch (m_BoatState)
	{
	case BoatState::SPAWNED:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Valeur de l'attribut : %f"), m_BoatState));
		FString tag("WaitPoint_Start_Water");
		AActor* actor = FindActorWithTag(tag, false);


		if (actor != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Valeur de l'attribut : %f"), m_BoatState));
			// TODO : Agents wants to move towards actor
			GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, TEXT("Vu"));
			ComputeAndFollowPathToTarget(actor->GetActorLocation());
			m_ReachedTarget = false;

			m_BoatState = BoatState::GO_TO_START_BRIDGE;
		}


		break;
	}
	case BoatState::GO_TO_START_BRIDGE:
	{
		// Nothing to do
		break;
	}
	case BoatState::WAIT_AT_START_BRIDGE:
	{
		// Check if bridge is down
		FString tag("Bridge_0");
		AActor* actor = FindActorWithTag(tag, false);
		ASDTBridge* bridge = Cast<ASDTBridge>(actor);

		// Once the bridge is down, we go through
		if (bridge != nullptr && bridge->GetState() == EBridgeState::BRIDGE_UP)
		{
			m_BoatState = BoatState::GO_TO_OPERATOR;
		}
	

		break;
	}
	case BoatState::GO_TO_OPERATOR:
	{
		FString tag("Bridge_0");
		AActor* actorBridge = FindActorWithTag(tag, false);
		ASDTBridge* bridge = Cast<ASDTBridge>(actorBridge);
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(this, ASDTBoatOperator::StaticClass(), foundActors);


		for (AActor* actor : foundActors)
		{
			
			ASDTBoatOperator* boatOperator = Cast<ASDTBoatOperator>(actor);
			//
			// boatOperator->Deactivate();
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Nombre d'acteurs trouvés : %s"), *(boatOperator->GetDropLocation()).ToString()));


			/*if (bridge && bridge->GetState() != EBridgeState::BRIDGE_UP) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("don t go"));
				if (boatOperator) {
					boatOperator->ClearReservation();
				}
				m_BoatState = BoatState::WAIT_AT_START_BRIDGE;
				m_ReachedTarget = true;
				this->reserved = false;
				break;
			}*/


			if (boatOperator->IsAvailable() && !this->reserved && bridge && bridge->GetState() == EBridgeState::BRIDGE_UP)
			{



				boatOperator->Reserve(this);
				ComputeAndFollowPathToTarget(boatOperator->GetDropLocation());
				m_BoatState = BoatState::GO_TO_OPERATOR;
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("operator?s"));
				m_ReachedTarget = false;
				this->reserved = true;


				







				// TODO : we want to move the agent towards the DropLocation of the boatOperator 
				// Check ASDTBoatOperator::GetDropLocation to get the location.
				// Note that m_ReachedTarget should be set to FALSE if the move is valid!

				break;

			}
			

		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("ALLLLLLLLLLLLL REEEEEESSSSSSSSSERVED"));
		//m_BoatState = BoatState::WAIT_AT_START_BRIDGE;

		/*if (!operatorFound && allOperatorsReserved)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Tous les opérateurs sont réservés. En attente..."));
			m_BoatState = BoatState::WAIT_AT_OPERATOR;
		}*/
		/*		if (operatorFound==0) {
					m_BoatState = BoatState::WAIT_AT_OPERATOR;

				}*/
		break;
	}
	case BoatState::WAIT_AT_OPERATOR:
	{

		// Nothing to do, wait for notification to advance

		break;
	}
	case BoatState::GO_TO_END_BRIDGE:
	{
		// Nothing to do
		break;
	}
	case BoatState::WAIT_AT_END_BRIDGE:
	{
		// Check if bridge is down
		FString tag("Bridge_1");
		AActor* actor = FindActorWithTag(tag, false);
		ASDTBridge* bridge = Cast<ASDTBridge>(actor);

		// Once the bridge is down, we go through
		if (bridge != nullptr && bridge->GetState() == EBridgeState::BRIDGE_UP)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Bridge is UP, moving to GO_TO_DESPAWN"));
			m_BoatState = BoatState::GO_TO_DESPAWN;

			tag = "WaitPoint_Water_2";
			actor = FindActorWithTag(tag, false);
			if (actor != nullptr)
			{
				ComputeAndFollowPathToTarget(actor->GetActorLocation());
				//m_ReachedTarget = false;
				// TODO : Agents wants to move towards actor
			}


		}

		break;
	}
	case BoatState::GO_TO_DESPAWN:
	{

		// Nothing to do
		break;
	}
	case BoatState::DESPAWN:
	{

		UnPossess();
		Destroy();
		if (pawn)
			pawn->Destroy();
		break;
	}
	}
}

void ASDTBoatAIController::NotifyUnloadComplete()
{

	m_BoatState = BoatState::GO_TO_END_BRIDGE;
	m_ReachedTarget = false;

	FString tag("WaitPoint_End_Water");
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("aleer verzaderzfazddddgfdefazrgeezrgefef"));
	AActor* actor = FindActorWithTag(tag, false);
	if (actor != nullptr)
	{
		ComputeAndFollowPathToTarget(actor->GetActorLocation());
		// TODO : Agents wants to move towards actor
	}
}

void ASDTBoatAIController::ShowNavigationPath()
{


	UPathFollowingComponent* path = GetPathFollowingComponent();
	if (path == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("destination invalide"));
		return;
	}

	FNavPathSharedPtr navPath = path->GetPath();

	if (navPath.IsValid())
	{
		TArray<FNavPathPoint> pathPoints = navPath->GetPathPoints();
		for (int i = 0; i < pathPoints.Num() - 1; i++)
		{
			DrawDebugLine(GetWorld(), pathPoints[i].Location, pathPoints[i + 1].Location, FColor::Green, false, 2.5f);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("destination invalide"));
	}
	// Show current navigation path DrawDebugLine and DrawDebugSphere
	// Use the UPathFollowingComponent of the AIController to get the path
	// This function is called while m_ReachedTarget is false 
	// Check void ASDTBaseAIController::Tick for how it works.
}

BoatState ASDTBoatAIController::GetBoatState()
{
	return m_BoatState;
}

void ASDTBoatAIController::AIStateInterrupted()
{
	StopMovement();

	m_ReachedTarget = false;
}

void ASDTBoatAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	m_ReachedTarget = true;

	switch (m_BoatState)
	{
	case BoatState::GO_TO_START_BRIDGE:
	{
		m_BoatState = BoatState::WAIT_AT_START_BRIDGE;
		break;
	}
	case BoatState::GO_TO_OPERATOR:
	{

		m_BoatState = BoatState::WAIT_AT_OPERATOR;
		break;
	}
	case BoatState::GO_TO_END_BRIDGE:
	{
		m_BoatState = BoatState::WAIT_AT_END_BRIDGE;
		break;
	}
	case BoatState::GO_TO_DESPAWN:
	{
		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("arrrriveeer"));

		m_BoatState = BoatState::DESPAWN;
		break;
	}
	}
}


