// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayObjectInterface.h"
#include "GameplayObjectStateControllerComponent.generated.h"


/**
 * This component handles gameplay object related functionality, such as activation, deactivation, enabling, disabling,
 * and managing state transitions. 
 */
UCLASS(ClassGroup=(TrickyGameplayObjects), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API UGameplayObjectStateControllerComponent : public UActorComponent,
                                                                          public IGameplayObjectInterface
{
	GENERATED_BODY()

public:
	UGameplayObjectStateControllerComponent();

protected:
	virtual void InitializeComponent() override;

public:
	/**
	 * Triggered when CurrentState was changed.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnGameplayObjectStateChangedDynamicSignature OnGameplayObjectStateChanged;

	/**
	 * Triggered when CurrentState was changed to the Transition state.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnGameplayObjectStateTransitionStartedDynamicSignature OnGameplayObjectStateTransitionStarted;

	/**
	 * Triggered when CurrentState was changed from Transition to TargetState.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnGameplayObjectStateTransitionFinishedDynamicSignature OnGameplayObjectStateTransitionFinished;

	/**
	 * Triggered when TargetState was reversed (swapped) with LastState.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnGameplayObjectStateTransitionReversedDynamicSignature OnGameplayObjectStateTransitionReversed;

	UFUNCTION(BlueprintGetter, Category=GameplayObjectState)
	FORCEINLINE EGameplayObjectState GetInitialState() const { return InitialState; }

	UFUNCTION(BlueprintSetter, Category=GameplayObjectState)
	void SetInitialState(const EGameplayObjectState NewState);

	UFUNCTION(BlueprintGetter, Category=GameplayObjectState)
	FORCEINLINE EGameplayObjectState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintGetter, Category=GameplayObjectState)
	FORCEINLINE EGameplayObjectState GetTargetState() const { return TargetState; }

	UFUNCTION(BlueprintGetter, Category=GameplayObjectState)
	FORCEINLINE EGameplayObjectState GetLastState() const { return LastState; }

	virtual bool ActivateGameplayObject_Implementation(const bool bTransitImmediately) override;

	virtual bool DeactivateGameplayObject_Implementation(const bool bTransitImmediately) override;

	virtual bool DisableGameplayObject_Implementation(const bool bTransitImmediately) override;

	virtual bool EnableGameplayObject_Implementation(const bool bTransitImmediately) override;

	virtual bool ForceGameplayObjectState_Implementation(const EGameplayObjectState NewState,
	                                       const bool bTransitImmediately) override;

	virtual bool FinishGameplayObjetStateTransition_Implementation() override;

	virtual bool ReverseGameplayObjectStateTransition_Implementation() override;

private:
	/**
	 * Defines the initial state of the gameplay objects during the component's initialization.
	 * It cannot be set to Transition.
	 */
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetInitialState,
		BlueprintSetter=SetInitialState,
		Category=GameplayObjectState)
	EGameplayObjectState InitialState = EGameplayObjectState::Active;

	/**
	 * Tracks the current operational state of the gameplay objects.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentState, Category=GameplayObjectState)
	EGameplayObjectState CurrentState = EGameplayObjectState::Active;

	/**
	 * Represents the intended state that the gameplay objects should transition to.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=GameplayObjectState)
	EGameplayObjectState TargetState = EGameplayObjectState::Active;

	/**
	 * Holds the previous state of the gameplay objects before the last state transition.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetLastState, Category=GameplayObjectState)
	EGameplayObjectState LastState = EGameplayObjectState::Active;

	/**
	 * Changes the current state of the gameplay objects to the specified new state.
	 * Handles the logic for both immediate and transitional state changes.
	 *
	 * @param NewState The desired state to transition to.
	 * @param bTransitImmediately Determines if the transition should occur immediately or through a transitional phase.
	 * @return True if the state was successfully changed, false otherwise.
	 */
	UFUNCTION()
	bool ChangeCurrentState(const EGameplayObjectState NewState, const bool bTransitImmediately);
	
#if WITH_EDITOR || !UE_BUILD_SHIPPING
	void PrintWarning(const FString& Message) const;

	void PrintLog(const FString& Message) const;

	static void GetStateName(FString& StateName, const EGameplayObjectState State);
#endif
};
