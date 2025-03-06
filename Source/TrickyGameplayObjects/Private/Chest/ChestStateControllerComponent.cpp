// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Chest/ChestStateControllerComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogChest);

UChestStateControllerComponent::UChestStateControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UChestStateControllerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (InitialState == EChestState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("InitialState can't be Transition. Reset it to Closed");
#endif
		InitialState = EChestState::Closed;
	}

	CurrentState = InitialState;
}

void UChestStateControllerComponent::SetInitialState(const EChestState NewState)
{
	if (NewState == EChestState::Transition)
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

bool UChestStateControllerComponent::OpenChest_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EChestState::Closed)
	{
		return false;
	}

	return ChangeCurrentState(EChestState::Opened, bTransitImmediately);
}

bool UChestStateControllerComponent::CloseChest_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EChestState::Opened)
	{
		return false;
	}

	return ChangeCurrentState(EChestState::Closed, bTransitImmediately);
}

bool UChestStateControllerComponent::LockChest_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EChestState::Closed)
	{
		return false;
	}

	return ChangeCurrentState(EChestState::Locked, bTransitImmediately);
}

bool UChestStateControllerComponent::UnlockChest_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EChestState::Locked)
	{
		return false;
	}

	return ChangeCurrentState(EChestState::Closed, bTransitImmediately);
}

bool UChestStateControllerComponent::DisableChest_Implementation(const bool bTransitImmediately)
{
	if (CurrentState == EChestState::Disabled)
	{
		return false;
	}

	return ChangeCurrentState(EChestState::Disabled, bTransitImmediately);
}

bool UChestStateControllerComponent::EnableChest_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EChestState::Disabled)
	{
		return false;
	}

	return ChangeCurrentState(LastState, bTransitImmediately);
}

bool UChestStateControllerComponent::ForceChestState_Implementation(const EChestState NewState,
                                                                    const bool bTransitImmediately)
{
	if (CurrentState == NewState)
	{
		return false;
	}

	if (NewState == EChestState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't force state to Transition");
#endif
		return false;
	}

	return ChangeCurrentState(NewState, bTransitImmediately);
}

bool UChestStateControllerComponent::FinishChestStateTransition_Implementation()
{
	if (CurrentState != EChestState::Transition)
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

	OnChestStateTransitionFinished.Broadcast(this, TargetState);
	return true;
}

bool UChestStateControllerComponent::ReverseChestStateTransition_Implementation()
{
	if (CurrentState != EChestState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't reverse transition, because CurrentState is Transition");
#endif
		return false;
	}

	const EChestState NewTargetState = LastState;
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

	OnChestTransitionReversed.Broadcast(this, TargetState);
	return true;
}

bool UChestStateControllerComponent::ChangeCurrentState(EChestState NewState, const bool bTransitImmediately)
{
	if (NewState == CurrentState)
	{
		return false;
	}

	if (NewState == EChestState::Transition)
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
		CurrentState = EChestState::Transition;

#if WITH_EDITOR || !UE_BUILD_SHIPPING
		FString TargetStateName = "NONE";
		GetStateName(TargetStateName, TargetState);
		FString LastStateName = "NONE";
		GetStateName(LastStateName, LastState);
		const FString LogMessage = FString::Printf(
			TEXT("Start state transition from %s to %s"), *LastStateName, *TargetStateName);
		PrintLog(LogMessage);
#endif

		OnChestStateTransitionStarted.Broadcast(this, TargetState);
	}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	FString StateName = "NONE";
	GetStateName(StateName, CurrentState);
	const FString LogMessage = FString::Printf(TEXT("State changed to %s"), *StateName);
	PrintLog(LogMessage);
#endif

	OnChestStateChanged.Broadcast(this, CurrentState, bTransitImmediately);
	return true;
}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
void UChestStateControllerComponent::PrintWarning(const FString& Message) const
{
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *GetOwner()->GetActorNameOrLabel());
	UE_LOG(LogChest, Display, TEXT("%s%s"), *SourceMessage, *Message);
}

void UChestStateControllerComponent::PrintLog(const FString& Message) const
{
	const FString OwnerName = GetOwner() ? GetOwner()->GetActorNameOrLabel() : "NONE";
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *OwnerName);
	UE_LOG(LogChest, Display, TEXT("%s%s"), *SourceMessage, *Message);
}

void UChestStateControllerComponent::GetStateName(FString& StateName, const EChestState State)
{
	StateName = StaticEnum<EChestState>()->GetNameStringByValue(static_cast<int64>(State));
}
#endif
