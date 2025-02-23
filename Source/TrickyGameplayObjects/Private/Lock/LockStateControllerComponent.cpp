// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Lock/LockStateControllerComponent.h"
#include "GameFramework/Actor.h"


ULockStateControllerComponent::ULockStateControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void ULockStateControllerComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	if (InitialState == ELockState::Transition)
	{
		InitialState = ELockState::Locked;
	}

	CurrentState = InitialState;
}

void ULockStateControllerComponent::SetInitialState(const ELockState NewState)
{
	if (InitialState == ELockState::Transition)
	{
		return;
	}

	InitialState = NewState;
	CurrentState = InitialState;
}

bool ULockStateControllerComponent::Lock_Implementation(AActor* OtherActor,
                                                        const bool bTransitImmediately)
{
	if (CurrentState != ELockState::Unlocked)
	{
		return false;
	}

	if (!IsValid(OtherActor))
	{
		return false;
	}

	return ChangeCurrentState(ELockState::Locked, bTransitImmediately);
}

bool ULockStateControllerComponent::Unlock_Implementation(AActor* OtherActor,
                                                          const bool bTransitImmediately)
{
	if (CurrentState != ELockState::Locked)
	{
		return false;
	}

	if (!IsValid(OtherActor))
	{
		return false;
	}

	return ChangeCurrentState(ELockState::Unlocked, bTransitImmediately);
}

bool ULockStateControllerComponent::Disable_Implementation(const bool bTransitImmediately)
{
	if (CurrentState == ELockState::Disabled)
	{
		return false;
	}
	
	return ChangeCurrentState(ELockState::Disabled, bTransitImmediately);
}

bool ULockStateControllerComponent::Enable_Implementation(const bool bTransitImmediately)
{
	return true;
}

bool ULockStateControllerComponent::ForceState_Implementation(const ELockState NewState,
                                                              const bool bTransitImmediately)
{
	return true;
}

bool ULockStateControllerComponent::FinishStateTransition()
{
	if (CurrentState != ELockState::Transition)
	{
		return false;
	}
	
	if (!ChangeCurrentState(TargetState, true))
	{
		return false;
	}

	OnLockStateTransitionFinished.Broadcast(this, TargetState);
	return true;
}

bool ULockStateControllerComponent::ReverseTransition()
{
	if (CurrentState != ELockState::Transition)
	{
		return false;
	}

	return true;
}

bool ULockStateControllerComponent::ChangeCurrentState(const ELockState NewState, const bool bTransitImmediately)
{
	if (CurrentState == NewState)
	{
		return false;
	}

	if (NewState == ELockState::Transition)
	{
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
		CurrentState = ELockState::Transition;
		OnLockStateTransitionStarted.Broadcast(this, TargetState);
	}

	OnLockStateChanged.Broadcast(this, CurrentState, bTransitImmediately);
	return true;
}
