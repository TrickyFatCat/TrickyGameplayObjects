// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

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
	FOnDoorStateTransitionReversedDynamicSignature OnDoorTransitionReversed;

	UFUNCTION(BlueprintGetter, Category=DoorState)
	FORCEINLINE EDoorState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintGetter, Category=DoorState)
	FORCEINLINE EDoorState GetTargetState() const { return TargetState; }

	UFUNCTION(BlueprintGetter, Category=DoorState)
	FORCEINLINE EDoorState GetInitialState() const { return InitialState; }

	UFUNCTION(BlueprintGetter, Category=DoorState)
	FORCEINLINE EDoorState GetLastState() const { return LastState; }

	UFUNCTION(BlueprintSetter, Category=DoorState)
	void SetInitialState(const EDoorState NewState);

	virtual bool OpenDoor_Implementation(const bool bTransitImmediately) override;

	virtual bool CloseDoor_Implementation(const bool bTransitImmediately) override;

	virtual bool LockDoor_Implementation(const bool bTransitImmediately) override;

	virtual bool UnlockDoor_Implementation(const bool bTransitImmediately) override;

	virtual bool DisableDoor_Implementation(const bool bTransitImmediately) override;

	virtual bool EnableDoor_Implementation(const bool bTransitImmediately) override;

	virtual bool ForceDoorState_Implementation(const EDoorState NewState, const bool bTransitImmediately) override;

	virtual bool FinishDoorStateTransition_Implementation() override;

	virtual bool ReverseDoorStateTransition_Implementation() override;

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
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetLastState, Category=DoorState)
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
