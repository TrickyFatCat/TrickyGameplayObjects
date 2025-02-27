// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayObjectInterface.generated.h"

class UGameplayObjectStateControllerComponent;

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
 * 
 */
class TRICKYGAMEPLAYOBJECTS_API IGameplayObjectInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool ActivateGameplayObject(const bool bTransitImmediately);

	virtual bool ActivateGameplayObject_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool DeactivateGameplayObject(const bool bTransitImmediately);

	virtual bool DeactivateGameplayObject_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool DisableGameplayObject(const bool bTransitImmediately);

	virtual bool DisableGameplayObject_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool EnableGameplayObject(const bool bTransitImmediately);

	virtual bool EnableGameplayObject_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool ForceGameplayObjectState(const EGameplayObjectState NewState, const bool bTransitImmediately);

	virtual bool ForceGameplayObjectState_Implementation(const EGameplayObjectState NewState,
	                                                     const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool FinishGameplayObjetStateTransition();

	virtual bool FinishGameplayObjetStateTransition_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = GameplayObject)
	bool ReverseGameplayObjectStateTransition();

	virtual bool ReverseGameplayObjectStateTransition_Implementation();
};
