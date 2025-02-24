// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Lock/LockInterface.h"
#include "LockStateControllerComponent.generated.h"


class ULockKeyType;

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
	FORCEINLINE TSubclassOf<ULockKeyType> GetRequiredKey() const { return RequiredKey; }

	UFUNCTION(BlueprintSetter, Category=LockState)
	void SetRequiredKey(const TSubclassOf<ULockKeyType>& NewKey);
	
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

	bool Lock_Implementation(AActor* OtherActor, const bool bTransitImmediately) override;

	bool Unlock_Implementation(AActor* OtherActor, const bool bTransitImmediately) override;

	bool Disable_Implementation(const bool bTransitImmediately) override;

	bool Enable_Implementation(const bool bTransitImmediately) override;

	bool ForceState_Implementation(const ELockState NewState, const bool bTransitImmediately) override;

	bool FinishStateTransition_Implementation() override;

	bool ReverseStateTransition_Implementation() override;

private:
	UPROPERTY(EditInstanceOnly, BlueprintGetter=GetRequiredKey, BlueprintSetter=SetRequiredKey, Category=LockState)
	TSubclassOf<ULockKeyType> RequiredKey = nullptr;
	
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

	UFUNCTION()
	bool TryUseKeyFromActor(AActor* OtherActor);
};

