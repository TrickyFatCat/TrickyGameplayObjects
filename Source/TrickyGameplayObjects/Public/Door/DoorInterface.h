// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DoorInterface.generated.h"

class UDoorStateControllerComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogDoor, Log, All);

/**
 * Representing the various states a door can be in.
 */
UENUM(BlueprintType)
enum class EDoorState: uint8
{
	Closed,
	Opened,
	Locked,
	Disabled,
	Transition
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDoorStateChangedDynamicSignature,
                                               UDoorStateControllerComponent*, Component,
                                               EDoorState, NewState,
                                               bool, bChangedImmediately);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDoorStateTransitionStartedDynamicSignature,
                                             UDoorStateControllerComponent*, Component,
                                             EDoorState, TargetState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDoorStateTransitionFinishedDynamicSignature,
                                             UDoorStateControllerComponent*, Component,
                                             EDoorState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDoorStateTransitionReversedDynamicSignature,
                                             UDoorStateControllerComponent*, Component,
                                             EDoorState, NewTargetState);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UDoorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for managing door functionalities such as opening, closing, locking, unlocking, enabling, and disabling.
 * This interface is designed to standardize door interactions and transitions.
 */
class TRICKYGAMEPLAYOBJECTS_API IDoorInterface
{
	GENERATED_BODY()

public:
	/**
	 * Attempts to open the door. 
	 *
	 * @param bTransitImmediately Determines if the door should transit to the open state immediately or not.
	 * @return True if the door was successfully opened or started a transition to opened state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool OpenDoor(const bool bTransitImmediately);

	virtual bool OpenDoor_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to close the door.
	 *
	 * @param bTransitImmediately Determines if the door should transit to the closed state immediately or not.
	 * @return True if the door was successfully closed or started transition to the closed state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool CloseDoor(const bool bTransitImmediately);

	virtual bool CloseDoor_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to lock the door.
	 *
	 * @param bTransitImmediately Determines if the door should transit to the locked state immediately or not.
	 * @return True if the door was successfully locked or started a transition to the locked state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool LockDoor(const bool bTransitImmediately);

	virtual bool LockDoor_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to unlock the door.
	 *
	 * @param bTransitImmediately Determines if the door should transit to the unlocked state (usually closed) immediately or not.
	 * @return True if the door was successfully unlocked or started a transition from the unlocked to closed state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool UnlockDoor(const bool bTransitImmediately);

	virtual bool UnlockDoor_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to disable the door.
	 *
	 * @param bTransitImmediately Determines if the door should transit to the disabled state immediately or not.
	 * @return True if the door was successfully disabled or started a transition to the disabled state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool DisableDoor(const bool bTransitImmediately);

	virtual bool DisableDoor_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to enable the door.
	 * 
	 * @param bTransitImmediately Determines if the door should transit to the enabled (usually the last before disabling) state immediately or not.
	 * @return  True if the door was successfully enabled or started a transition from the enabled to last state before disabling.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool EnableDoor(const bool bTransitImmediately);

	virtual bool EnableDoor_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to force the door to enter specific state.
	 * @warning The Door can't be forced to Transition state.
	 *
	 * @param NewState The desired state to force the door into.
	 * @param bTransitImmediately Determines if the door should transit to NewState immediately or not.
	 * @return True if the door successfully changed the state or started a transition to NewState.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Door)
	bool ForceDoorState(const EDoorState NewState, const bool bTransitImmediately);

	virtual bool ForceDoorState_Implementation(const EDoorState NewState, const bool bTransitImmediately);

	/**
	 * Completes the current door state transition.
	 * @warning current state should be Transition.
	 *
	 * @return True if the state transition is successfully completed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool FinishDoorStateTransition();

	virtual bool FinishDoorStateTransition_Implementation();

	/**
	 * Reverses the current target state of the door with the last recorded state.
	 * @warning  current state should be Transition.
	 *
	 * @return True if the target state is successfully reversed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool ReverseDoorStateTransition();

	virtual bool ReverseDoorStateTransition_Implementation();
};
