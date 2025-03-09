// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Lock/LockStateControllerComponent.h"

#include "GameFramework/Actor.h"
#include "LockKey/KeyringComponent.h"
#include "LockKey/KeyringInterface.h"
#include "LockKey/LockKeyType.h"

DEFINE_LOG_CATEGORY(LogLock);

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
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("InitialState can't be Transition. Reset it to Locked");
#endif
		InitialState = ELockState::Locked;
	}

	CurrentState = InitialState;
}

void ULockStateControllerComponent::SetRequiredKey(const TSubclassOf<ULockKeyType>& NewKey)
{
	RequiredKey = NewKey;
}

void ULockStateControllerComponent::SetInitialState(const ELockState NewState)
{
	if (InitialState == ELockState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("InitialState can't be Transition. Didn't change the state.");
#endif
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

	if (!TryUseKeyFromActor(OtherActor))
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

	if (!TryUseKeyFromActor(OtherActor))
	{
		return false;
	}

	return ChangeCurrentState(ELockState::Unlocked, bTransitImmediately);
}

bool ULockStateControllerComponent::DisableLock_Implementation(const bool bTransitImmediately)
{
	if (CurrentState == ELockState::Disabled)
	{
		return false;
	}

	return ChangeCurrentState(ELockState::Disabled, bTransitImmediately);
}

bool ULockStateControllerComponent::EnableLock_Implementation(const bool bTransitImmediately)
{
	if (CurrentState != ELockState::Disabled)
	{
		return false;
	}

	return ChangeCurrentState(LastState, bTransitImmediately);
}

bool ULockStateControllerComponent::ForceLockState_Implementation(const ELockState NewState,
                                                              const bool bTransitImmediately)
{
	if (CurrentState == NewState)
	{
		return false;
	}

	if (NewState == ELockState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't force state to Transition");
#endif
		return false;
	}
	
	return ChangeCurrentState(NewState, bTransitImmediately);
}

bool ULockStateControllerComponent::FinishLockStateTransition_Implementation()
{
	if (CurrentState != ELockState::Transition)
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

	OnLockStateTransitionFinished.Broadcast(this, TargetState);
	return true;
}

bool ULockStateControllerComponent::ReverseLockStateTransition_Implementation()
{
	if (CurrentState != ELockState::Transition)
	{
#if WITH_EDITOR || !UE_BUILD_SHIPPING
		PrintWarning("Can't reverse transition, because CurrentState is Transition");
#endif
		return false;
	}

	const ELockState NewTargetState = LastState;
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

	OnLockTransitionReversed.Broadcast(this, TargetState);
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
		CurrentState = ELockState::Transition;

#if WITH_EDITOR || !UE_BUILD_SHIPPING
		FString TargetStateName = "NONE";
		GetStateName(TargetStateName, TargetState);
		FString LastStateName = "NONE";
		GetStateName(LastStateName, LastState);
		const FString LogMessage = FString::Printf(
			TEXT("Start state transition from %s to %s"), *LastStateName, *TargetStateName);
		PrintLog(LogMessage);
#endif

		OnLockStateTransitionStarted.Broadcast(this, TargetState);
	}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	FString StateName = "NONE";
	GetStateName(StateName, CurrentState);
	const FString LogMessage = FString::Printf(TEXT("State changed to %s"), *StateName);
	PrintLog(LogMessage);
#endif
	
	OnLockStateChanged.Broadcast(this, CurrentState, bTransitImmediately);
	return true;
}

bool ULockStateControllerComponent::TryUseKeyFromActor(const AActor* OtherActor)
{
	if (!IsValid(OtherActor) || !IsValid(RequiredKey))
	{
		return false;
	}

	UKeyringComponent* KeyringComponent = OtherActor->FindComponentByClass<UKeyringComponent>();
	
	if (!IsValid(KeyringComponent))
	{
		return false;
	}

	return IKeyringInterface::Execute_UseLockKey(KeyringComponent, RequiredKey);
}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
void ULockStateControllerComponent::PrintWarning(const FString& Message) const
{
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *GetOwner()->GetActorNameOrLabel());
	UE_LOG(LogLock, Warning, TEXT("%s%s"), *SourceMessage, *Message);
}

void ULockStateControllerComponent::PrintLog(const FString& Message) const
{
	const FString OwnerName = GetOwner() ? GetOwner()->GetActorNameOrLabel() : "NONE";
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *OwnerName);
	UE_LOG(LogLock, Display, TEXT("%s%s"), *SourceMessage, *Message);
}

void ULockStateControllerComponent::GetStateName(FString& StateName, const ELockState State)
{
	StateName = StaticEnum<ELockState>()->GetNameStringByValue(static_cast<int64>(State));
}
#endif
