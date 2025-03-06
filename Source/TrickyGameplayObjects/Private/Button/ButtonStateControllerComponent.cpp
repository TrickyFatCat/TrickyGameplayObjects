// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Button/ButtonStateControllerComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogButton);

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
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("InitialState can't be Transition. Reset it to Closed");
#endif
		InitialState = EButtonState::Released;
	}

	CurrentState = InitialState;
}

void UButtonStateControllerComponent::SetInitialState(const EButtonState NewState)
{
	if (NewState == EButtonState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("InitialState can't be Transition. Didn't change the state.");
#endif
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
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't force state to Transition");
#endif
		return false;
	}

	return ChangeCurrentState(NewState, bTransitImmediately);
}

bool UButtonStateControllerComponent::FinishButtonStateTransition_Implementation()
{
	if (CurrentState != EButtonState::Transition)
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

	OnButtonStateTransitionFinished.Broadcast(this, TargetState);
	return true;
}

bool UButtonStateControllerComponent::ReverseButtonStateTransition_Implementation()
{
	if (CurrentState != EButtonState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't reverse transition, because CurrentState is Transition");
#endif
		return false;
	}

	const EButtonState NewTargetState = LastState;
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
		CurrentState = EButtonState::Transition;

#if WITH_EDITOR || !UE_BUILD_SHIPPING
		FString TargetStateName = "NONE";
		GetStateName(TargetStateName, TargetState);
		FString LastStateName = "NONE";
		GetStateName(LastStateName, LastState);
		const FString LogMessage = FString::Printf(
			TEXT("Start state transition from %s to %s"), *LastStateName, *TargetStateName);
		PrintLog(LogMessage);
#endif
		
		OnButtonStateTransitionStarted.Broadcast(this, TargetState);
	}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	FString StateName = "NONE";
	GetStateName(StateName, CurrentState);
	const FString LogMessage = FString::Printf(TEXT("State changed to %s"), *StateName);
	PrintLog(LogMessage);
#endif
	
	OnButtonStateChanged.Broadcast(this, CurrentState, bTransitImmediately);
	return true;
}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
void UButtonStateControllerComponent::PrintWarning(const FString& Message) const
{
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *GetOwner()->GetActorNameOrLabel());
	UE_LOG(LogButton, Warning, TEXT("%s%s"), *SourceMessage, *Message);
}

void UButtonStateControllerComponent::PrintLog(const FString& Message) const
{
	const FString OwnerName = GetOwner() ? GetOwner()->GetActorNameOrLabel() : "NONE";
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *OwnerName);
	UE_LOG(LogButton, Display, TEXT("%s%s"), *SourceMessage, *Message);
}

void UButtonStateControllerComponent::GetStateName(FString& StateName, const EButtonState State)
{
	StateName = StaticEnum<EButtonState>()->GetNameStringByValue(static_cast<int64>(State));
}
#endif
