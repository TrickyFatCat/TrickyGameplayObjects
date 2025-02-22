﻿// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "DoorInterface.h"
#include "Components/ActorComponent.h"
#include "DoorStateControllerComponent.generated.h"

/**
 * This component handles door-related functionality, such as opening, closing, locking, unlocking, enabling, disabling,
 * and managing state transitions. 
 */
UCLASS(Blueprintable, ClassGroup=(TrickyGameplayObjects), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API UDoorStateControllerComponent : public UActorComponent, public IDoorInterface
{
	GENERATED_BODY()

public:
	UDoorStateControllerComponent();

protected:
	virtual void InitializeComponent() override;

public:
	/**
	 * Triggered when CurrentState of the door was changed.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnDoorStateChangedDynamicSignature OnDoorStateChanged;

	/**
	 * Triggered when CurrentState was changed to the Transition state.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnDoorStateTransitionStartedDynamicSignature OnDoorStateTransitionStarted;

	/**
	 * Triggered when CurrentState was changed from Transition to TargetState.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnDoorStateTransitionFinishedDynamicSignature OnDoorStateTransitionFinished;

	/**
	 * Triggered when TargetState was reversed (swapped) with LastState.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnDoorTransitionReversedDynamicSignature OnDoorTransitionReversed;

	UFUNCTION(BlueprintGetter, Category=DoorState)
	FORCEINLINE EDoorState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintGetter, Category=DoorState)
	FORCEINLINE EDoorState GetTargetState() const { return TargetState; }

	UFUNCTION(BlueprintGetter, Category=DoorState)
	FORCEINLINE EDoorState GetInitialState() const { return InitialState; }
	
	UFUNCTION(BlueprintCallable, Category=DoorState)
	void SetInitialState(const EDoorState NewState);

	bool OpenDoor_Implementation(const bool bTransitImmediately);

	bool CloseDoor_Implementation(const bool bTransitImmediately);

	bool LockDoor_Implementation(const bool bTransitImmediately);

	bool UnlockDoor_Implementation(const bool bTransitImmediately);

	bool DisableDoor_Implementation(const bool bTransitImmediately);

	bool EnableDoor_Implementation(const bool bTransitImmediately);

	bool EnableDoorForced_Implementation(const EDoorState NewState, const bool bTransitImmediately);

	/**
	 * Completes the current door state transition if CurrentState is Transition.
	 * Automatically sets CurrentState to TargetState
	 *
	 * @return True if the state transition is successfully completed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category=DoorState)
	bool FinishStateTransition();

	/**
	 * Reverses the current target state of the door with the last recorded state if CurrentState is Transition.
	 * Effectively swapping TargetState and LastState.
	 *
	 * @return True if the target state is successfully reversed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category=DoorState)
	bool ReverseTransition();

private:
	/**
	 * Defines the initial state of the door during the component's initialization.
	 * It cannot be set to Transition.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetInitialState, BlueprintSetter=SetInitialState, Category=DoorState)
	EDoorState InitialState = EDoorState::Closed;

	/**
	 * Tracks the current operational state of the door.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentState, Category=DoorState)
	EDoorState CurrentState = EDoorState::Closed;


	/**
	 * Represents the intended state that the door should transition to.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=DoorState)
	EDoorState TargetState = EDoorState::Closed;


	/**
	 * Holds the previous state of the door before the last state transition.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=DoorState)
	EDoorState LastState = EDoorState::Closed;

	/**
	 * Changes the current state of the door to the specified new state.
	 * Handles the logic for both immediate and transitional state changes.
	 *
	 * @param NewState The desired state to transition to.
	 * @param bTransitImmediately Determines if the transition should occur immediately or through a transitional phase.
	 * @return True if the state was successfully changed, false otherwise.
	 */
	UFUNCTION()
	bool ChangeCurrentState(EDoorState NewState, const bool bTransitImmediately);

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	void PrintWarning(const FString& Message) const;

	void PrintLog(const FString& Message) const;

	static void GetStateName(FString& StateName, const EDoorState State);
#endif
};

