// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LockInterface.generated.h"

class ULockStateControllerComponent;
class AActor;

DECLARE_LOG_CATEGORY_EXTERN(LogLock, Log, All);

UENUM(BlueprintType)
enum class ELockState: uint8
{
	Locked,
	Unlocked,
	Disabled,
	Transition
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLockStateChangedDynamicSignature,
                                               ULockStateControllerComponent*, Component,
                                               ELockState, NewState,
                                               bool, bChangedImmediately);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockStateTransitionStartedDynamicSignature,
                                             ULockStateControllerComponent*, Component,
                                             ELockState, TargetState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockStateTransitionFinishedDynamicSignature,
                                             ULockStateControllerComponent*, Component,
                                             ELockState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockStateTransitionReversedDynamicSignature,
                                             ULockStateControllerComponent*, Component,
                                             ELockState, NewTargetState);

// This class does not need to be modified.
UINTERFACE()
class ULockInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TRICKYGAMEPLAYOBJECTS_API ILockInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool Lock(AActor* OtherActor, const bool bTransitImmediately);

	virtual bool Lock_Implementation(AActor* OtherActor, const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool Unlock(AActor* OtherActor, const bool bTransitImmediately);

	virtual bool Unlock_Implementation(AActor* OtherActor, const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool Disable(const bool bTransitImmediately);

	virtual bool Disable_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool Enable(const bool bTransitImmediately);

	virtual bool Enable_Implementation(const bool bTransitImmediately);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Lock)
	bool ForceState(const ELockState NewState, const bool bTransitImmediately);

	virtual bool ForceState_Implementation(const ELockState NewState, const bool bTransitImmediately);
};
