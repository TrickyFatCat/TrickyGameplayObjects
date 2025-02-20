// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "DoorInterface.h"
#include "Components/ActorComponent.h"
#include "DoorStateControllerComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(TrickyGameplayObjects), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API UDoorStateControllerComponent : public UActorComponent, public IDoorInterface
{
	GENERATED_BODY()

public:
	UDoorStateControllerComponent();

protected:
	virtual void InitializeComponent() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnDoorStateChangedDynamicSignature OnDoorStateChanged;

	UPROPERTY(BlueprintAssignable)
	FOnDoorStateTransitionStartedDynamicSignature OnDoorStateTransitionStarted;

	UPROPERTY(BlueprintAssignable)
	FOnDoorTransitionReversedDynamicSignature OnDoorTransitionReversed;

	UFUNCTION(BlueprintGetter)
	FORCEINLINE EDoorState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE EDoorState GetTargetState() const { return TargetState; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE EDoorState GetInitialState() const { return InitialState; }
	
	UFUNCTION(BlueprintCallable)
	void SetInitialState(const EDoorState NewState);

	bool OpenDoor_Implementation(const bool bTransitImmediately);

	bool CloseDoor_Implementation(const bool bTransitImmediately);

	bool LockDoor_Implementation(const bool bTransitImmediately);

	bool UnlockDoor_Implementation(const bool bTransitImmediately);

	bool DisableDoor_Implementation(const bool bTransitImmediately);

	bool EnableDoor_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable)
	bool FinishStateTransition();

	UFUNCTION(BlueprintCallable)
	bool ReverseTransition();

private:
	UPROPERTY(EditAnywhere, BlueprintGetter=GetInitialState, BlueprintSetter=SetInitialState, Category=DoorState)
	EDoorState InitialState = EDoorState::Closed;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetCurrentState, Category=DoorState)
	EDoorState CurrentState = EDoorState::Closed;

	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=DoorState)
	EDoorState TargetState = EDoorState::Closed;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetTargetState, Category=DoorState)
	EDoorState LastState = EDoorState::Closed;

	UFUNCTION()
	bool ChangeCurrentState(EDoorState NewState, const bool bTransitImmediately);

	void PrintWarning(const FString& Message);
};
