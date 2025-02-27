// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "GameplayObject/GameplayObjectStateControllerComponent.h"


UGameplayObjectStateControllerComponent::UGameplayObjectStateControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UGameplayObjectStateControllerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (InitialState == EGameplayObjectState::Transition)
	{
		InitialState = EGameplayObjectState::Active;
	}

	CurrentState = InitialState;
}

void UGameplayObjectStateControllerComponent::SetInitialState(const EGameplayObjectState NewState)
{
	if (NewState == EGameplayObjectState::Transition)
	{
		return;
	}

	if (InitialState == NewState)
	{
		return;
	}

	InitialState = NewState;
	CurrentState = InitialState;
}

bool UGameplayObjectStateControllerComponent::ActivateGameplayObject_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EGameplayObjectState::Inactive)
	{
		return false;
	}

	return ChangeCurrentState(EGameplayObjectState::Active, bTransitImmediately);
}

bool UGameplayObjectStateControllerComponent::DeactivateGameplayObject_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EGameplayObjectState::Active)
	{
		return false;
	}

	return ChangeCurrentState(EGameplayObjectState::Inactive, bTransitImmediately);
}

bool UGameplayObjectStateControllerComponent::DisableGameplayObject_Implementation(const bool bTransitImmediately)
{
	if (CurrentState == EGameplayObjectState::Disabled)
	{
		return false;
	}

	return ChangeCurrentState(EGameplayObjectState::Disabled, bTransitImmediately);
}

bool UGameplayObjectStateControllerComponent::EnableGameplayObject_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EGameplayObjectState::Disabled)
	{
		return false;
	}

	return ChangeCurrentState(LastState, bTransitImmediately);
}

bool UGameplayObjectStateControllerComponent::ForceGameplayObjectState_Implementation(const EGameplayObjectState NewState,
                                                                        const bool bTransitImmediately)
{
	if (CurrentState == NewState)
	{
		return false;
	}

	if (NewState == EGameplayObjectState::Transition)
	{
		return false;
	}

	return ChangeCurrentState(NewState, bTransitImmediately);
}

bool UGameplayObjectStateControllerComponent::FinishGameplayObjetStateTransition_Implementation()
{
	if (CurrentState != EGameplayObjectState::Transition)
	{
		return false;
	}

	return ChangeCurrentState(TargetState, true);
}

bool UGameplayObjectStateControllerComponent::ReverseGameplayObjectStateTransition_Implementation()
{
	if (CurrentState != EGameplayObjectState::Transition)
	{
		return false;
	}

	const EGameplayObjectState NewTargetState = LastState;
	LastState = TargetState;
	TargetState = NewTargetState;
	OnGameplayObjectStateTransitionReversed.Broadcast(this, TargetState);
	return true;
}

bool UGameplayObjectStateControllerComponent::ChangeCurrentState(const EGameplayObjectState NewState,
                                                                 const bool bTransitImmediately)
{
	if (NewState == CurrentState)
	{
		return false;
	}

	if (NewState == EGameplayObjectState::Transition)
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
		CurrentState = EGameplayObjectState::Transition;
		OnGameplayObjectStateTransitionStarted.Broadcast(this, TargetState);
	}

	OnGameplayObjectStateChanged.Broadcast(this, CurrentState, bTransitImmediately);
	return true;
}
