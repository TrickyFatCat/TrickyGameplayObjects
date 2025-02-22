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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDoorTransitionReversedDynamicSignature,
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

	/**
	 * Attempts to close the door.
	 *
	 * @param bTransitImmediately Determines if the door should transit to the closed state immediately or not.
	 * @return True if the door was successfully closed or started transition to the closed state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool CloseDoor(const bool bTransitImmediately);

	/**
	 * Attempts to lock the door.
	 *
	 * @param bTransitImmediately Determines if the door should transit to the locked state immediately or not.
	 * @return True if the door was successfully locked or started a transition to the locked state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool LockDoor(const bool bTransitImmediately);

	/**
	 * Attempts to unlock the door.
	 *
	 * @param bTransitImmediately Determines if the door should transit to the unlocked state (usually closed) immediately or not.
	 * @return True if the door was successfully unlocked or started a transition from the unlocked to closed state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool UnlockDoor(const bool bTransitImmediately);

	/**
	 * Attempts to disable the door.
	 *
	 * @param bTransitImmediately Determines if the door should transit to the disabled state immediately or not.
	 * @return True if the door was successfully disabled or started a transition to the disabled state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool DisableDoor(const bool bTransitImmediately);

	/**
	 * Attempts to enable the door.
	 * 
	 * @param bTransitImmediately Determines if the door should transit to the enabled (usually the last before disabling) state immediately or not.
	 * @return  True if the door was successfully enabled or started a transition from the enabled to last state before disabling.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Door)
	bool EnableDoor(const bool bTransitImmediately);

	/**
	 * Attempts to enable the door to a specific state. 
	 *
	 * @param NewState The desired state to force the door into.
	 * @param bTransitImmediately Determines if the door should transit to NewState immediately or not.
	 * @return True if the door successfully changed the state to NewState or started a transition to NewState.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Door)
	bool EnableDoorForced(const EDoorState NewState, const bool bTransitImmediately);
};
