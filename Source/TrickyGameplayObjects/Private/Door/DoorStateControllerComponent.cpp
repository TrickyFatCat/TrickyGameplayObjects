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
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("InitialState can't be Transition. Reset it to Closed");
#endif
		InitialState = EDoorState::Closed;
	}

	CurrentState = InitialState;
}

void UDoorStateControllerComponent::SetInitialState(const EDoorState NewState)
{
	if (NewState == EDoorState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("InitialState can't be Transition. Didn't change the state.");
#endif
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

bool UDoorStateControllerComponent::ForceDoorState_Implementation(const EDoorState NewState,
                                                              const bool bTransitImmediately)
{
	if (CurrentState == NewState)
	{
		return false;
	}

	if (NewState == EDoorState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't force state to Transition");
#endif
		return false;
	}

	return ChangeCurrentState(NewState, bTransitImmediately);
}

bool UDoorStateControllerComponent::FinishDoorStateTransition_Implementation()
{
	if (CurrentState != EDoorState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't finish transition, because CurrentState isn't Transition");
#endif
		return false;
	}

	if (!ChangeCurrentState(TargetState, true))
	{
		return false;
	}

	OnDoorStateTransitionFinished.Broadcast(this, TargetState);
	return true;
}

bool UDoorStateControllerComponent::ReverseDoorStateTransition_Implementation()
{
	if (CurrentState != EDoorState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't reverse transition, because CurrentState is Transition");
#endif
		return false;
	}

	const EDoorState NewTargetState = LastState;
	LastState = TargetState;
	TargetState = NewTargetState;

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	FString TargetStateName = "NONE";
	GetStateName(TargetStateName, TargetState);
	FString LastStateName = "NONE";
	GetStateName(LastStateName, LastState);
	const FString LogMessage = FString::Printf(
		TEXT("TargetState reversed from %s to %s"), *LastStateName, *TargetStateName);
	PrintLog(LogMessage);
#endif

	OnDoorTransitionReversed.Broadcast(this, TargetState);
	return true;
}

bool UDoorStateControllerComponent::ChangeCurrentState(const EDoorState NewState, const bool bTransitImmediately)
{
	if (NewState == CurrentState)
	{
		return false;
	}

	if (NewState == EDoorState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't change CurrentState to Transition");
#endif

		return false;
	}

	TargetState = NewState;
	LastState = CurrentState;

	if (bTransitImmediately)
	{
		CurrentState = TargetState;
	}
	else
	{
		CurrentState = EDoorState::Transition;

#if WITH_EDITOR || !UE_BUILD_SHIPPING
		FString TargetStateName = "NONE";
		GetStateName(TargetStateName, TargetState);
		FString LastStateName = "NONE";
		GetStateName(LastStateName, LastState);
		const FString LogMessage = FString::Printf(
			TEXT("Start state transition from %s to %s"), *LastStateName, *TargetStateName);
		PrintLog(LogMessage);
#endif

		OnDoorStateTransitionStarted.Broadcast(this, TargetState);
	}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	FString StateName = "NONE";
	GetStateName(StateName, CurrentState);
	const FString LogMessage = FString::Printf(TEXT("State changed to %s"), *StateName);
	PrintLog(LogMessage);
#endif

	OnDoorStateChanged.Broadcast(this, CurrentState, bTransitImmediately);
	return true;
}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
void UDoorStateControllerComponent::PrintWarning(const FString& Message) const
{
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *GetOwner()->GetActorNameOrLabel());
	UE_LOG(LogDoor, Warning, TEXT("%s%s"), *SourceMessage, *Message);
}

void UDoorStateControllerComponent::PrintLog(const FString& Message) const
{
	const FString OwnerName = GetOwner() ? GetOwner()->GetActorNameOrLabel() : "NONE";
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *OwnerName);
	UE_LOG(LogDoor, Display, TEXT("%s%s"), *SourceMessage, *Message);
}

void UDoorStateControllerComponent::GetStateName(FString& StateName, const EDoorState State)
{
	StateName = StaticEnum<EDoorState>()->GetNameStringByValue(static_cast<int64>(State));
}
#endif
