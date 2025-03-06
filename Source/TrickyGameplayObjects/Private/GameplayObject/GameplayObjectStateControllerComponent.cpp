// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "GameplayObject/GameplayObjectStateControllerComponent.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogGameplayObject)

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
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("InitialState can't be Transition. Reset it to Closed");
#endif
		InitialState = EGameplayObjectState::Active;
	}

	CurrentState = InitialState;
}

void UGameplayObjectStateControllerComponent::SetInitialState(const EGameplayObjectState NewState)
{
	if (NewState == EGameplayObjectState::Transition)
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
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't force state to Transition");
#endif
		return false;
	}

	return ChangeCurrentState(NewState, bTransitImmediately);
}

bool UGameplayObjectStateControllerComponent::FinishGameplayObjetStateTransition_Implementation()
{
	if (CurrentState != EGameplayObjectState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't finish transition, because CurrentState isn't Transition");
#endif
		return false;
	}

	return ChangeCurrentState(TargetState, true);
}

bool UGameplayObjectStateControllerComponent::ReverseGameplayObjectStateTransition_Implementation()
{
	if (CurrentState != EGameplayObjectState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't reverse transition, because CurrentState is Transition");
#endif
		return false;
	}

	const EGameplayObjectState NewTargetState = LastState;
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
		CurrentState = EGameplayObjectState::Transition;

#if WITH_EDITOR || !UE_BUILD_SHIPPING
		FString TargetStateName = "NONE";
		GetStateName(TargetStateName, TargetState);
		FString LastStateName = "NONE";
		GetStateName(LastStateName, LastState);
		const FString LogMessage = FString::Printf(
			TEXT("Start state transition from %s to %s"), *LastStateName, *TargetStateName);
		PrintLog(LogMessage);
#endif
		
		OnGameplayObjectStateTransitionStarted.Broadcast(this, TargetState);
	}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	FString StateName = "NONE";
	GetStateName(StateName, CurrentState);
	const FString LogMessage = FString::Printf(TEXT("State changed to %s"), *StateName);
	PrintLog(LogMessage);
#endif
	
	OnGameplayObjectStateChanged.Broadcast(this, CurrentState, bTransitImmediately);
	return true;
}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
void UGameplayObjectStateControllerComponent::PrintWarning(const FString& Message) const
{
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *GetOwner()->GetActorNameOrLabel());
	UE_LOG(LogGameplayObject, Warning, TEXT("%s%s"), *SourceMessage, *Message);
}

void UGameplayObjectStateControllerComponent::PrintLog(const FString& Message) const
{
	const FString OwnerName = GetOwner() ? GetOwner()->GetActorNameOrLabel() : "NONE";
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *OwnerName);
	UE_LOG(LogGameplayObject, Display, TEXT("%s%s"), *SourceMessage, *Message);
}

void UGameplayObjectStateControllerComponent::GetStateName(FString& StateName, const EGameplayObjectState State)
{
	StateName = StaticEnum<EGameplayObjectState>()->GetNameStringByValue(static_cast<int64>(State));
}
#endif
