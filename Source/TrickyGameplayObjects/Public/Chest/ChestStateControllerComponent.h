// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ChestInterface.h"
#include "ChestStateControllerComponent.generated.h"


UCLASS(ClassGroup=(TrickyGameplayObjects), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API UChestStateControllerComponent : public UActorComponent, public IChestInterface
{
	GENERATED_BODY()

public:
	UChestStateControllerComponent();

protected:
	virtual void InitializeComponent() override;

public:
	/**
	 * Triggered when CurrentState of the chest was changed.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnChestStateChangedDynamicSignature OnChestStateChanged;

	/**
	 * Triggered when CurrentState was changed to the Transition state.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnChestStateTransitionStartedDynamicSignature OnChestStateTransitionStarted;

	/**
	 * Triggered when CurrentState was changed from Transition to TargetState.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnChestStateTransitionFinishedDynamicSignature OnChestStateTransitionFinished;

	/**
	 * Triggered when TargetState was reversed (swapped) with LastState.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnChestStateTransitionReversedDynamicSignature OnChestTransitionReversed;

	UFUNCTION(BlueprintGetter, Category=ChestState)
	FORCEINLINE EChestState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintGetter, Category=ChestState)
	FORCEINLINE EChestState GetTargetState() const { return TargetState; }

	UFUNCTION(BlueprintGetter, Category=ChestState)
	FORCEINLINE EChestState GetInitialState() const { return InitialState; }

	UFUNCTION(BlueprintGetter, Category=ChestState)
	FORCEINLINE EChestState GetLastState() const { return LastState; }

	UFUNCTION(BlueprintSetter, Category=ChestState)
	void SetInitialState(const EChestState NewState);

	virtual bool OpenChest_Implementation(const bool bTransitImmediately) override;

	virtual bool CloseChest_Implementation(const bool bTransitImmediately) override;

	virtual bool LockChest_Implementation(const bool bTransitImmediately) override;

	virtual bool UnlockChest_Implementation(const bool bTransitImmediately) override;

	virtual bool DisableChest_Implementation(const bool bTransitImmediately) override;

	virtual bool EnableChest_Implementation(const bool bTransitImmediately) override;

	virtual bool ForceChestState_Implementation(const EChestState NewState, const bool bTransitImmediately) override;

	virtual bool FinishChestStateTransition_Implementation() override;

	virtual bool ReverseChestStateTransition_Implementation() override;

private:
	/**
	 * Defines the initial state of the chest during the component's initialization.
	 * It cannot be set to Transition.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetInitialState, BlueprintSetter=SetInitialState, Category=ChestState)
	EChestState InitialState = EChestState::Closed;

	/**
	 * Tracks the current operational state of the chest.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentState, Category=ChestState)
	EChestState CurrentState = EChestState::Closed;

	/**
	 * Represents the intended state that the chest should transition to.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=ChestState)
	EChestState TargetState = EChestState::Closed;

	/**
	 * Holds the previous state of the chest before the last state transition.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetLastState, Category=ChestState)
	EChestState LastState = EChestState::Closed;

	/**
	 * Changes the current state of the chest to the specified new state.
	 * Handles the logic for both immediate and transitional state changes.
	 *
	 * @param NewState The desired state to transition to.
	 * @param bTransitImmediately Determines if the transition should occur immediately or through a transitional phase.
	 * @return True if the state was successfully changed, false otherwise.
	 */
	UFUNCTION()
	bool ChangeCurrentState(EChestState NewState, const bool bTransitImmediately);

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	void PrintWarning(const FString& Message) const;

	void PrintLog(const FString& Message) const;

	static void GetStateName(FString& StateName, const EChestState State);
#endif
};
