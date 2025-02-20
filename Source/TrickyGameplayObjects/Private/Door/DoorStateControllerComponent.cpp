// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Door/DoorStateControllerComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogDoor);

UDoorStateControllerComponent::UDoorStateControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UDoorStateControllerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (InitialState == EDoorState::Transition)
	{
		PrintWarning("InitialState can't be EDoorState::Transition. Reset it to EDoorState::Closed");
		InitialState = EDoorState::Closed;
	}

	CurrentState = InitialState;
}

void UDoorStateControllerComponent::SetInitialState(const EDoorState NewState)
{
	if (NewState == EDoorState::Transition)
	{
		PrintWarning("InitialState can't be EDoorState::Transition. Didn't change the state.");
		return;
	}

	if (InitialState == NewState)
	{
		return;
	}

	InitialState = NewState;
	CurrentState = InitialState;
}

bool UDoorStateControllerComponent::OpenDoor_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EDoorState::Closed)
	{
		return false;
	}

	return ChangeCurrentState(EDoorState::Opened, bTransitImmediately);
}

bool UDoorStateControllerComponent::CloseDoor_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EDoorState::Opened)
	{
		return false;
	}

	return ChangeCurrentState(EDoorState::Closed, bTransitImmediately);
}

bool UDoorStateControllerComponent::LockDoor_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EDoorState::Closed)
	{
		return false;
	}

	return ChangeCurrentState(EDoorState::Locked, bTransitImmediately);
}

bool UDoorStateControllerComponent::UnlockDoor_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EDoorState::Locked)
	{
		return false;
	}

	return ChangeCurrentState(EDoorState::Closed, bTransitImmediately);
}

bool UDoorStateControllerComponent::DisableDoor_Implementation(const bool bTransitImmediately)
{
	if (CurrentState == EDoorState::Disabled)
	{
		return false;
	}

	return ChangeCurrentState(EDoorState::Disabled, bTransitImmediately);
}

bool UDoorStateControllerComponent::EnableDoor_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EDoorState::Disabled)
	{
		return false;
	}

	return ChangeCurrentState(LastState, bTransitImmediately);
}

bool UDoorStateControllerComponent::FinishStateTransition()
{
	if (CurrentState != EDoorState::Transition)
	{
		UE_LOG(LogDoor, Warning, TEXT("Can't finish transition, because CurrentState isn't EDoorState::Transition"))
		return false;
	}

	return ChangeCurrentState(TargetState, true);
}

bool UDoorStateControllerComponent::ReverseTransition()
{
	if (CurrentState == EDoorState::Transition)
	{
		UE_LOG(LogDoor, Warning, TEXT("Can't reverse transition, because CurrentState is EDoorState::Transition"))
		return false;
	}

	const EDoorState NewTargetState = LastState;
	LastState = TargetState;
	TargetState = NewTargetState;
	OnDoorTransitionReversed.Broadcast(this, TargetState);
	return true;
}

bool UDoorStateControllerComponent::ChangeCurrentState(const EDoorState NewState, const bool bTransitImmediately)
{
	if (NewState == CurrentState)
	{
		return false;
	}

	TargetState = NewState;

	if (bTransitImmediately)
	{
		LastState = CurrentState;
		CurrentState = TargetState;
	}
	else
	{
		LastState = CurrentState;
		CurrentState = EDoorState::Transition;
		OnDoorStateTransitionStarted.Broadcast(this, TargetState);
	}

	OnDoorStateChanged.Broadcast(this, CurrentState, bTransitImmediately);
	return true;
}

void UDoorStateControllerComponent::PrintWarning(const FString& Message)
{
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *GetOwner()->GetActorNameOrLabel());
	UE_LOG(LogDoor, Warning, TEXT("%s%s"), *SourceMessage, *Message);
}
