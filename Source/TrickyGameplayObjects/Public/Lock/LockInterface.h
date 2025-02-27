// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LockInterface.generated.h"

class ULockStateControllerComponent;
class AActor;

DECLARE_LOG_CATEGORY_EXTERN(LogLock, Log, All);

/**
 * Represents the various states of a lock.
 */
UENUM(BlueprintType)
enum class ELockState: uint8
{
	Locked,
	Unlocked,
	Disabled,
	Transition
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLockStateChangedDynamicSignature,
                                               ULockStateControllerComponent*, Component,
                                               ELockState, NewState,
                                               bool, bChangedImmediately);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockStateTransitionStartedDynamicSignature,
                                             ULockStateControllerComponent*, Component,
                                             ELockState, TargetState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockStateTransitionFinishedDynamicSignature,
                                             ULockStateControllerComponent*, Component,
                                             ELockState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockStateTransitionReversedDynamicSignature,
                                             ULockStateControllerComponent*, Component,
                                             ELockState, NewTargetState);

// This class does not need to be modified.
UINTERFACE()
class ULockInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for managing lock functionalities such as locking, unlocking, enabling, and disabling.
 * This interface is designed to standardize lock interactions and transitions.
 */
class TRICKYGAMEPLAYOBJECTS_API ILockInterface
{
	GENERATED_BODY()

public:
	/**
	 * Attempts to lock a lock using a component (which implements KeyRingInterface) from a given actor.
	 *
	 * @param OtherActor The actor that interacts with the lock and has KeyRingComponent.
	 * @param bTransitImmediately Determines if the lock should transit to the locked state immediately or not.
	 * @return Returns true if the lock was successfully locked on started a transition to locked state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool Lock(AActor* OtherActor, const bool bTransitImmediately);

	virtual bool Lock_Implementation(AActor* OtherActor, const bool bTransitImmediately);

	/**
	 * Attempts to unlock a lock using a component (which implements KeyRingInterface) from a given actor.
	 *
	 * @param OtherActor The actor that interacts with the lock and has KeyRingComponent.
	 * @param bTransitImmediately Determines if the lock should transit to the unlocked state immediately or not.
	 * @return Returns true if the lock was successfully unlocked on started a transition to unlocked state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool Unlock(AActor* OtherActor, const bool bTransitImmediately);

	virtual bool Unlock_Implementation(AActor* OtherActor, const bool bTransitImmediately);

	/**
	 * Attempts to disable a lock.
	 *
	 * @param bTransitImmediately Determines if the lock should transit to the disabled state immediately or not.
	 * @return Returns true if the lock was successfully disabled on started a transition to disabled state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool DisableLock(const bool bTransitImmediately);

	virtual bool DisableLock_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to enable a lock.
	 *
	 * @param bTransitImmediately Determines if the lock should transit to the enabled (usually the last before disabling) state immediately or not.
	 * @return  True if the lock was successfully enabled or started a transition from the enabled to last state before disabling.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool EnableLock(const bool bTransitImmediately);

	virtual bool EnableLock_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to force a lock to enter specific state.
	 * @warning The lock can't be forced to Transition state.
	 *
	 * @param NewState The desired state to force the lock into.
	 * @param bTransitImmediately Determines if the lock should transit to NewState immediately or not.
	 * @return True if the lock successfully changed the state or started a transition to NewState.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool ForceLockState(const ELockState NewState, const bool bTransitImmediately);

	virtual bool ForceLockState_Implementation(const ELockState NewState, const bool bTransitImmediately);

	/**
	 * Completes the current lock state transition.
	 * @warning current state should be Transition.
	 *
	 * @return True if the state transition is successfully completed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool FinishLockStateTransition();

	virtual bool FinishLockStateTransition_Implementation();

	/**
	 * Reverses the current target state of the lock with the last recorded state.
	 * @warning  current state should be Transition.
	 *
	 * @return True if the target state is successfully reversed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool ReverseLockStateTransition();

	virtual bool ReverseLockStateTransition_Implementation();
};
