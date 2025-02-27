// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Lock/LockInterface.h"
#include "LockStateControllerComponent.generated.h"


class ULockKeyType;

/**
 * This component handles lock-related functionality, such as locking, unlocking, enabling, disabling,
 * and managing state transitions. 
 */
UCLASS(ClassGroup=(TrickyGameplayObjects), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API ULockStateControllerComponent : public UActorComponent, public ILockInterface
{
	GENERATED_BODY()

public:
	ULockStateControllerComponent();

protected:
	virtual void InitializeComponent() override;

public:
	/**
	 * Triggered when CurrentState of the lock was changed.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnLockStateChangedDynamicSignature OnLockStateChanged;

	/**
	 * Triggered when CurrentState was changed to the Transition state.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnLockStateTransitionStartedDynamicSignature OnLockStateTransitionStarted;

	/**
	 * Triggered when CurrentState was changed from Transition to TargetState.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnLockStateTransitionFinishedDynamicSignature OnLockStateTransitionFinished;

	/**
	 * Triggered when TargetState was reversed (swapped) with LastState.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnLockStateTransitionReversedDynamicSignature OnLockTransitionReversed;

	UFUNCTION(BlueprintGetter, Category=LockState)
	FORCEINLINE TSubclassOf<ULockKeyType> GetRequiredKey() const { return RequiredKey; }

	UFUNCTION(BlueprintSetter, Category=LockState)
	void SetRequiredKey(const TSubclassOf<ULockKeyType>& NewKey);

	UFUNCTION(BlueprintGetter, Category=LockState)
	FORCEINLINE ELockState GetInitialState() const { return InitialState; }

	UFUNCTION(BlueprintSetter, Category=LockState)
	void SetInitialState(const ELockState NewState);

	UFUNCTION(BlueprintGetter, Category=LockState)
	FORCEINLINE ELockState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintGetter, Category=LockState)
	FORCEINLINE ELockState GetTargetState() const { return TargetState; }

	UFUNCTION(BlueprintGetter, Category=LockState)
	FORCEINLINE ELockState GetLastState() const { return LastState; }

	bool Lock_Implementation(AActor* OtherActor, const bool bTransitImmediately) override;

	bool Unlock_Implementation(AActor* OtherActor, const bool bTransitImmediately) override;

	bool DisableLock_Implementation(const bool bTransitImmediately) override;

	bool EnableLock_Implementation(const bool bTransitImmediately) override;

	bool ForceLockState_Implementation(const ELockState NewState, const bool bTransitImmediately) override;

	bool FinishLockStateTransition_Implementation() override;

	bool ReverseLockStateTransition_Implementation() override;

private:
	/**
	 * Specifies the key type required to interact with the lock.
	 */
	UPROPERTY(EditInstanceOnly, BlueprintGetter=GetRequiredKey, BlueprintSetter=SetRequiredKey, Category=LockState)
	TSubclassOf<ULockKeyType> RequiredKey = nullptr;

	/**
	 * Defines the initial state of the lock during the component's initialization.
	 * It cannot be set to Transition.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetInitialState, BlueprintSetter=SetInitialState, Category=LockState)
	ELockState InitialState = ELockState::Locked;

	/**
	 * Tracks the current operational state of the lock.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentState, Category=LockState)
	ELockState CurrentState = ELockState::Locked;

	/**
	 * Represents the intended state that the lock should transition to.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=LockState)
	ELockState TargetState = ELockState::Locked;

	/**
	 * Holds the previous state of the lock before the last state transition.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetLastState, Category=LockState)
	ELockState LastState = ELockState::Locked;

	/**
	 * Changes the current state of the lock to the specified new state.
	 * Handles the logic for both immediate and transitional state changes.
	 *
	 * @param NewState The desired state to transition to.
	 * @param bTransitImmediately Determines if the transition should occur immediately or through a transitional phase.
	 * @return True if the state was successfully changed, false otherwise.
	 */
	UFUNCTION()
	bool ChangeCurrentState(const ELockState NewState, const bool bTransitImmediately);

	/**
	 * Attempts to use a required key in component (which implements KeyRingInterface) from a given actor.
	 */
	UFUNCTION()
	bool TryUseKeyFromActor(const AActor* OtherActor);

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	void PrintWarning(const FString& Message) const;

	void PrintLog(const FString& Message) const;

	static void GetStateName(FString& StateName, const ELockState State);
#endif
};
