// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ButtonInterface.h"
#include "ButtonStateControllerComponent.generated.h"


/**
 * This component handles button-related functionality, such as pressing, releasing, enabling, disabling,
 * and managing state transitions. 
 */
UCLASS(ClassGroup=(TrickyGameplayObjects), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API UButtonStateControllerComponent : public UActorComponent, public IButtonInterface
{
	GENERATED_BODY()

public:
	UButtonStateControllerComponent();

protected:
	virtual void InitializeComponent() override;

public:
	/**
	 * Triggered when CurrentState was changed.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnButtonStateChangedDynamicSignature OnButtonStateChanged;

	/**
	 * Triggered when CurrentState was changed to the Transition state.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnButtonStateTransitionStartedDynamicSignature OnButtonStateTransitionStarted;

	/**
	 * Triggered when CurrentState was changed from Transition to TargetState.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnButtonStateTransitionFinishedDynamicSignature OnButtonStateTransitionFinished;

	/**
	 * Triggered when TargetState was reversed (swapped) with LastState.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnButtonStateTransitionReversedDynamicSignature OnButtonStateTransitionReversed;

	UFUNCTION(BlueprintGetter, Category=ButtonState)
	FORCEINLINE EButtonState GetInitialState() const { return InitialState; }

	UFUNCTION(BlueprintSetter, Category=ButtonState)
	void SetInitialState(const EButtonState NewState);

	UFUNCTION(BlueprintGetter, Category=ButtonState)
	FORCEINLINE EButtonState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintGetter, Category=ButtonState)
	FORCEINLINE EButtonState GetTargetState() const { return TargetState; }

	UFUNCTION(BlueprintGetter, Category=ButtonState)
	FORCEINLINE EButtonState GetLastState() const { return LastState; }

	virtual bool PressButton_Implementation(const bool bTransitImmediately) override;

	virtual bool ReleaseButton_Implementation(const bool bTransitImmediately) override;

	virtual bool DisableButton_Implementation(const bool bTransitImmediately) override;

	virtual bool EnableButton_Implementation(const bool bTransitImmediately) override;

	virtual bool ForceButtonState_Implementation(const EButtonState NewState, const bool bTransitImmediately) override;

	virtual bool FinishButtonStateTransition_Implementation() override;

	virtual bool ReverseButtonStateTransition_Implementation() override;

private:
	/**
	 * Defines the initial state of the button during the component's initialization.
	 * It cannot be set to Transition.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetInitialState, BlueprintSetter=SetInitialState, Category=ButtonState)
	EButtonState InitialState = EButtonState::Released;

	/**
	 * Tracks the current operational state of the button.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentState, Category=ButtonState)
	EButtonState CurrentState = EButtonState::Released;

	/**
	 * Represents the intended state that the button should transition to.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=ButtonState)
	EButtonState TargetState = EButtonState::Released;

	/**
	 * Holds the previous state of the button before the last state transition.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetLastState, Category=ButtonState)
	EButtonState LastState = EButtonState::Released;

	/**
	 * Changes the current state of the button to the specified new state.
	 * Handles the logic for both immediate and transitional state changes.
	 *
	 * @param NewState The desired state to transition to.
	 * @param bTransitImmediately Determines if the transition should occur immediately or through a transitional phase.
	 * @return True if the state was successfully changed, false otherwise.
	 */
	UFUNCTION()
	bool ChangeCurrentState(const EButtonState NewState, const bool bTransitImmediately);

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	void PrintWarning(const FString& Message) const;

	void PrintLog(const FString& Message) const;

	static void GetStateName(FString& StateName, const EButtonState State);
#endif
};
