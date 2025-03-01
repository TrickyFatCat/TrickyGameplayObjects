// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ButtonInterface.generated.h"

class UButtonStateControllerComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogButton, Log, All)

/**
 * Representing the various states a button can be in.
 */
UENUM()
enum class EButtonState : uint8
{
	Released,
	Pressed,
	Disabled,
	Transition
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnButtonStateChangedDynamicSignature,
                                               UButtonStateControllerComponent*, Component,
                                               EButtonState, NewState,
                                               bool, bChangedImmediately);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonStateTransitionStartedDynamicSignature,
                                             UButtonStateControllerComponent*, Component,
                                             EButtonState, TargetState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonStateTransitionFinishedDynamicSignature,
                                             UButtonStateControllerComponent*, Component,
                                             EButtonState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonStateTransitionReversedDynamicSignature,
                                             UButtonStateControllerComponent*, Component,
                                             EButtonState, NewTargetState);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UButtonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for managing button functionalities such as pressing, releasing, enabling, and disabling.
 * This interface is designed to standardize button interactions and transitions.
 */
class TRICKYGAMEPLAYOBJECTS_API IButtonInterface
{
	GENERATED_BODY()

public:
	/**
	 * Attempts to press the button. 
	 *
	 * @param bTransitImmediately Determines if the button should transit to the open state immediately or not.
	 * @return True if the button was successfully pressed or started a transition to pressed state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool PressButton(const bool bTransitImmediately);

	virtual bool PressButton_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to release the button.
	 *
	 * @param bTransitImmediately Determines if the button should transit to the released state immediately or not.
	 * @return True if the button was successfully released or started transition to the released state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool ReleaseButton(const bool bTransitImmediately);

	virtual bool ReleaseButton_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to disable the button.
	 *
	 * @param bTransitImmediately Determines if the button should transit to the disabled state immediately or not.
	 * @return True if the button was successfully disabled or started a transition to the disabled state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool DisableButton(const bool bTransitImmediately);

	virtual bool DisableButton_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to enable the button.
	 * 
	 * @param bTransitImmediately Determines if the button should transit to the enabled (usually the last before disabling) state immediately or not.
	 * @return  True if the button was successfully enabled or started a transition from the enabled to last state before disabling.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool EnableButton(const bool bTransitImmediately);

	virtual bool EnableButton_Implementation(const bool bTransitImmediately);

	/**
	 * Completes the current button state transition.
	 * @warning current state should be Transition.
	 *
	 * @return True if the state transition is successfully completed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool ForceButtonState(const EButtonState NewState, const bool bTransitImmediately);

	virtual bool ForceButtonState_Implementation(const EButtonState NewState, const bool bTransitImmediately);

	/**
	 * Completes the current button state transition.
	 * @warning current state should be Transition.
	 *
	 * @return True if the state transition is successfully completed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool FinishButtonStateTransition();

	virtual bool FinishButtonStateTransition_Implementation();

	/**
	 * Reverses the current target state of the button with the last recorded state.
	 * @warning  current state should be Transition.
	 *
	 * @return True if the target state is successfully reversed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool ReverseButtonStateTransition();

	virtual bool ReverseButtonStateTransition_Implementation();
};
