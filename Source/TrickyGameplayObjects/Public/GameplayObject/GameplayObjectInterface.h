// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayObjectInterface.generated.h"

class UGameplayObjectStateControllerComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogGameplayObject, Log, All);

/**
 * Representing the various states a generic gameplay object can be in.
 */
UENUM(BlueprintType)
enum class EGameplayObjectState : uint8
{
	Active,
	Inactive,
	Disabled,
	Transition
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGameplayObjectStateChangedDynamicSignature,
                                               UGameplayObjectStateControllerComponent*, Component,
                                               EGameplayObjectState, NewState,
                                               bool, bChangedImmediately);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayObjectStateTransitionStartedDynamicSignature,
                                             UGameplayObjectStateControllerComponent*, Component,
                                             EGameplayObjectState, TargetState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayObjectStateTransitionFinishedDynamicSignature,
                                             UGameplayObjectStateControllerComponent*, Component,
                                             EGameplayObjectState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameplayObjectStateTransitionReversedDynamicSignature,
                                             UGameplayObjectStateControllerComponent*, Component,
                                             EGameplayObjectState, NewTargetState);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UGameplayObjectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for managing generic gameplay object functionalities such as activation, deactivation, enabling, and disabling.
 * This interface is designed to standardize generic gameplay object interactions and transitions.
 */
class TRICKYGAMEPLAYOBJECTS_API IGameplayObjectInterface
{
	GENERATED_BODY()

public:
	/**
	 * Attempts to activate the gameplay object. 
	 *
	 * @param bTransitImmediately Determines if the gameplay object should transit to the active state immediately or not.
	 * @return True if the gameplay object was successfully activated or started a transition to activated state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool ActivateGameplayObject(const bool bTransitImmediately);

	virtual bool ActivateGameplayObject_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to close the gameplay object.
	 *
	 * @param bTransitImmediately Determines if the gameplay object should transit to the deactivated state immediately or not.
	 * @return True if the gameplay object was successfully deactivated or started transition to the deactivated state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool DeactivateGameplayObject(const bool bTransitImmediately);

	virtual bool DeactivateGameplayObject_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to disable the gameplay object.
	 *
	 * @param bTransitImmediately Determines if the gameplay object should transit to the disabled state immediately or not.
	 * @return True if the gameplay object was successfully disabled or started a transition to the disabled state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool DisableGameplayObject(const bool bTransitImmediately);

	virtual bool DisableGameplayObject_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to enable the gameplay object.
	 * 
	 * @param bTransitImmediately Determines if the gameplay object should transit to the enabled (usually the last before disabling) state immediately or not.
	 * @return  True if the gameplay object was successfully enabled or started a transition from the enabled to last state before disabling.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool EnableGameplayObject(const bool bTransitImmediately);

	virtual bool EnableGameplayObject_Implementation(const bool bTransitImmediately);

	/**
	 * Attempts to force the gameplay object to enter specific state.
	 * @warning The gameplay object can't be forced to Transition state.
	 *
	 * @param NewState The desired state to force the gameplay object into.
	 * @param bTransitImmediately Determines if the gameplay object should transit to NewState immediately or not.
	 * @return True if the gameplay object successfully changed the state or started a transition to NewState.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool ForceGameplayObjectState(const EGameplayObjectState NewState, const bool bTransitImmediately);

	virtual bool ForceGameplayObjectState_Implementation(const EGameplayObjectState NewState,
	                                                     const bool bTransitImmediately);

	/**
	 * Completes the current gameplay object state transition.
	 * @warning current state should be Transition.
	 *
	 * @return True if the state transition is successfully completed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool FinishGameplayObjetStateTransition();

	virtual bool FinishGameplayObjetStateTransition_Implementation();

	/**
	 * Reverses the current target state of the gameplay object with the last recorded state.
	 * @warning  current state should be Transition.
	 *
	 * @return True if the target state is successfully reversed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool ReverseGameplayObjectStateTransition();

	virtual bool ReverseGameplayObjectStateTransition_Implementation();
};
