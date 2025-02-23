// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Lock/LockInterface.h"
#include "LockStateControllerComponent.generated.h"


UCLASS(ClassGroup=(TrickyGameplayObjects), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API ULockStateControllerComponent : public UActorComponent, public ILockInterface
{
	GENERATED_BODY()

public:
	ULockStateControllerComponent();

protected:
	virtual void InitializeComponent() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnLockStateChangedDynamicSignature OnLockStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnLockStateTransitionStartedDynamicSignature OnLockStateTransitionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnLockStateTransitionFinishedDynamicSignature OnLockStateTransitionFinished;

	UPROPERTY(BlueprintAssignable)
	FOnLockStateTransitionReversedDynamicSignature OnLockTransitionReversed;
	
	UFUNCTION(BlueprintGetter, Category=LockState)
	FORCEINLINE ELockState GetInitialState() const { return InitialState; }

	UFUNCTION(BlueprintSetter, Category=LockState)
	void SetInitialState(const ELockState NewState);
	
	UFUNCTION(BlueprintGetter, Category=LockState)
	FORCEINLINE ELockState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintGetter, Category=LockState)
	FORCEINLINE ELockState GetTargetState() const { return TargetState; }

	UFUNCTION(BlueprintGetter, Category=LockState)
	FORCEINLINE ELockState GetLastState() const { return LastState; }

	bool Lock_Implementation(AActor* OtherActor, const bool bIgnoreKey, const bool bTransitImmediately);

	bool Unlock_Implementation(AActor* OtherActor,const bool bIgnoreKey, const bool bTransitImmediately);

	bool Disable_Implementation(const bool bTransitImmediately);

	bool Enable_Implementation(const bool bTransitImmediately);

	bool ForceState_Implementation(const ELockState NewState, const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, Category=LockState)
	bool FinishStateTransition();

	UFUNCTION(BlueprintCallable, Category=LockState)
	bool ReverseTransition();

private:
	UPROPERTY(EditAnywhere, BlueprintGetter=GetInitialState, BlueprintSetter=SetInitialState, Category=LockState)
	ELockState InitialState = ELockState::Locked;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentState, Category=LockState)
	ELockState CurrentState = ELockState::Locked;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=LockState)
	ELockState TargetState = ELockState::Locked;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetLastState, Category=LockState)
	ELockState LastState = ELockState::Locked;
	
	UFUNCTION()
	bool ChangeCurrentState(const ELockState NewState, const bool bTransitImmediately);
};

