// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ButtonInterface.h"
#include "ButtonStateControllerComponent.generated.h"


UCLASS(ClassGroup=(TrickyGameplayObjects), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API UButtonStateControllerComponent : public UActorComponent, public IButtonInterface
{
	GENERATED_BODY()

public:
	UButtonStateControllerComponent();

protected:
	virtual void InitializeComponent() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnButtonStateChangedDynamicSignature OnButtonStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnButtonStateTransitionStartedDynamicSignature OnButtonStateTransitionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnButtonStateTransitionFinishedDynamicSignature OnButtonStateTransitionFinished;

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
	UPROPERTY(EditAnywhere, BlueprintGetter=GetInitialState, BlueprintSetter=SetInitialState, Category=ButtonState)
	EButtonState InitialState = EButtonState::Released;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentState, Category=ButtonState)
	EButtonState CurrentState = EButtonState::Released;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=ButtonState)
	EButtonState TargetState = EButtonState::Released;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetLastState, Category=ButtonState)
	EButtonState LastState = EButtonState::Released;

	UFUNCTION()
	bool ChangeCurrentState(const EButtonState NewState, const bool bTransitImmediately);
};
