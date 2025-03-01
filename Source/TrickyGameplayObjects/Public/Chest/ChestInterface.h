// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ChestInterface.generated.h"

class UChestStateControllerComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogChest, Log, All)

/**
 * Representing the various states a chest can be in.
 */
UENUM(BlueprintType)
enum class EChestState: uint8
{
	Closed,
	Opened,
	Locked,
	Disabled,
	Transition
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnChestStateChangedDynamicSignature,
                                               UChestStateControllerComponent*, Component,
                                               EChestState, NewState,
                                               bool, bChangedImmediately);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChestStateTransitionStartedDynamicSignature,
                                             UChestStateControllerComponent*, Component,
                                             EChestState, TargetState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChestStateTransitionFinishedDynamicSignature,
                                             UChestStateControllerComponent*, Component,
                                             EChestState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChestStateTransitionReversedDynamicSignature,
                                             UChestStateControllerComponent*, Component,
                                             EChestState, NewTargetState);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UChestInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TRICKYGAMEPLAYOBJECTS_API IChestInterface
{
	GENERATED_BODY()

public:
	/**
	 * Attempts to open the chest. 
	 *
	 * @param bTransitImmediately Determines if the chest should transit to the open state immediately or not.
	 * @return True if the chest was successfully opened or started a transition to opened state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Chest)
	bool OpenChest(const bool bTransitImmediately);

	virtual bool OpenChest_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to close the chest.
	 *
	 * @param bTransitImmediately Determines if the chest should transit to the closed state immediately or not.
	 * @return True if the chest was successfully closed or started transition to the closed state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Chest)
	bool CloseChest(const bool bTransitImmediately);

	virtual bool CloseChest_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to lock the chest.
	 *
	 * @param bTransitImmediately Determines if the chest should transit to the locked state immediately or not.
	 * @return True if the chest was successfully locked or started a transition to the locked state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Chest)
	bool LockChest(const bool bTransitImmediately);

	virtual bool LockChest_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to unlock the chest.
	 *
	 * @param bTransitImmediately Determines if the chest should transit to the unlocked state (usually closed) immediately or not.
	 * @return True if the chest was successfully unlocked or started a transition from the unlocked to closed state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Chest)
	bool UnlockChest(const bool bTransitImmediately);

	virtual bool UnlockChest_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to disable the chest.
	 *
	 * @param bTransitImmediately Determines if the chest should transit to the disabled state immediately or not.
	 * @return True if the chest was successfully disabled or started a transition to the disabled state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Chest)
	bool DisableChest(const bool bTransitImmediately);

	virtual bool DisableChest_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to enable the chest.
	 * 
	 * @param bTransitImmediately Determines if the chest should transit to the enabled (usually the last before disabling) state immediately or not.
	 * @return  True if the chest was successfully enabled or started a transition from the enabled to last state before disabling.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Chest)
	bool EnableChest(const bool bTransitImmediately);

	virtual bool EnableChest_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to force the chest to enter specific state.
	 * @warning The Chest can't be forced to Transition state.
	 *
	 * @param NewState The desired state to force the chest into.
	 * @param bTransitImmediately Determines if the chest should transit to NewState immediately or not.
	 * @return True if the chest successfully changed the state or started a transition to NewState.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category=Chest)
	bool ForceChestState(const EChestState NewState, const bool bTransitImmediately);

	virtual bool ForceChestState_Implementation(const EChestState NewState, const bool bTransitImmediately);

	/**
	 * Completes the current chest state transition.
	 * @warning current state should be Transition.
	 *
	 * @return True if the state transition is successfully completed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Chest)
	bool FinishChestStateTransition();

	virtual bool FinishChestStateTransition_Implementation();

	/**
	 * Reverses the current target state of the chest with the last recorded state.
	 * @warning  current state should be Transition.
	 *
	 * @return True if the target state is successfully reversed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Chest)
	bool ReverseChestStateTransition();

	virtual bool ReverseChestStateTransition_Implementation();
};
