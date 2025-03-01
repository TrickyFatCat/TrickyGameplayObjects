// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Button/ButtonStateControllerComponent.h"


UButtonStateControllerComponent::UButtonStateControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UButtonStateControllerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (InitialState == EButtonState::Transition)
	{
		InitialState = EButtonState::Released;
	}

	CurrentState = InitialState;
}

void UButtonStateControllerComponent::SetInitialState(const EButtonState NewState)
{
	if (NewState == EButtonState::Transition)
	{
		return;
	}

	if (CurrentState == NewState)
	{
		return;
	}

	InitialState = NewState;
	CurrentState = InitialState;
}

bool UButtonStateControllerComponent::PressButton_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EButtonState::Released)
	{
		return false;
	}

	return ChangeCurrentState(EButtonState::Pressed, bTransitImmediately);
}

bool UButtonStateControllerComponent::ReleaseButton_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EButtonState::Pressed)
	{
		return false;
	}

	return ChangeCurrentState(EButtonState::Released, bTransitImmediately);
}

bool UButtonStateControllerComponent::DisableButton_Implementation(const bool bTransitImmediately)
{
	if (CurrentState == EButtonState::Disabled)
	{
		return false;
	}

	return ChangeCurrentState(EButtonState::Disabled, bTransitImmediately);
}

bool UButtonStateControllerComponent::EnableButton_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != EButtonState::Disabled)
	{
		return false;
	}

	return ChangeCurrentState(LastState, bTransitImmediately);
}

bool UButtonStateControllerComponent::ForceButtonState_Implementation(const EButtonState NewState,
                                                                      const bool bTransitImmediately)
{
	if (CurrentState == NewState)
	{
		return false;
	}

	if (NewState == EButtonState::Transition)
	{
		return false;
	}

	return ChangeCurrentState(NewState, bTransitImmediately);
}

bool UButtonStateControllerComponent::FinishButtonStateTransition_Implementation()
{
	if (CurrentState != EButtonState::Transition)
	{
		return false;
	}

	if (!ChangeCurrentState(TargetState, true))
	{
		return false;
	}

	OnButtonStateTransitionFinished.Broadcast(this, TargetState);
	return true;
}

bool UButtonStateControllerComponent::ReverseButtonStateTransition_Implementation()
{
	if (CurrentState != EButtonState::Transition)
	{
		return false;
	}

	const EButtonState NewTargetState = LastState;
	LastState = TargetState;
	TargetState = NewTargetState;

	OnButtonStateTransitionReversed.Broadcast(this, TargetState);
	return true;
}

bool UButtonStateControllerComponent::ChangeCurrentState(const EButtonState NewState, const bool bTransitImmediately)
{
	if (NewState == CurrentState)
	{
		return false;
	}

	if (NewState == EButtonState::Transition)
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
		CurrentState = EButtonState::Transition;

		OnButtonStateTransitionStarted.Broadcast(this, TargetState);
	}

	OnButtonStateChanged.Broadcast(this, CurrentState, bTransitImmediately);
	return true;
}
