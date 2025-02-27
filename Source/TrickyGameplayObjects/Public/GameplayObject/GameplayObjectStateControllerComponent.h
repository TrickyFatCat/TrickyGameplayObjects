// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayObjectInterface.h"
#include "GameplayObjectStateControllerComponent.generated.h"


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
	UPROPERTY(BlueprintAssignable)
	FOnGameplayObjectStateChangedDynamicSignature OnGameplayObjectStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnGameplayObjectStateTransitionStartedDynamicSignature OnGameplayObjectStateTransitionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnGameplayObjectStateTransitionFinishedDynamicSignature OnGameplayObjectStateTransitionFinished;

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
	UPROPERTY(EditAnywhere,
		BlueprintGetter=GetInitialState,
		BlueprintSetter=SetInitialState,
		Category=GameplayObjectState)
	EGameplayObjectState InitialState = EGameplayObjectState::Active;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentState, Category=GameplayObjectState)
	EGameplayObjectState CurrentState = EGameplayObjectState::Active;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=GameplayObjectState)
	EGameplayObjectState TargetState = EGameplayObjectState::Active;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetLastState, Category=GameplayObjectState)
	EGameplayObjectState LastState = EGameplayObjectState::Active;

	UFUNCTION()
	bool ChangeCurrentState(const EGameplayObjectState NewState, const bool bTransitImmediately);
};
