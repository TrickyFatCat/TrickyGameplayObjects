// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ButtonInterface.generated.h"

class UButtonStateControllerComponent;
DECLARE_LOG_CATEGORY_EXTERN(LogButton, Log, All)

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
 * 
 */
class TRICKYGAMEPLAYOBJECTS_API IButtonInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool PressButton(const bool bTransitImmediately);

	virtual bool PressButton_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool ReleaseButton(const bool bTransitImmediately);

	virtual bool ReleaseButton_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool DisableButton(const bool bTransitImmediately);

	virtual bool DisableButton_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool EnableButton(const bool bTransitImmediately);

	virtual bool EnableButton_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool ForceButtonState(const EButtonState NewState, const bool bTransitImmediately);

	virtual bool ForceButtonState_Implementation(const EButtonState NewState, const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool FinishButtonStateTransition();

	virtual bool FinishButtonStateTransition_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Button)
	bool ReverseButtonStateTransition();

	virtual bool ReverseButtonStateTransition_Implementation();
};
