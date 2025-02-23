﻿// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "KeyRingInterface.h"
#include "Components/ActorComponent.h"
#include "KeyRingComponent.generated.h"


UCLASS(ClassGroup=(TrickyGameplayObjcets), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API UKeyRingComponent : public UActorComponent, public IKeyRingInterface
{
	GENERATED_BODY()

public:
	UKeyRingComponent();

protected:
	virtual void InitializeComponent() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnLockKeyAddedDynamicSignature OnLockKeyAdded;

	UPROPERTY(BlueprintAssignable)
	FOnLockKeyRemovedDynamicSignature OnLockKeyRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnAllLockKeysRemovedDynmaicSignature OnAllLockKeysRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnLockKeyUsedDynamicSignature OnLockKeyUsed;

	UFUNCTION(BlueprintGetter, Category=KeyRing)
	FORCEINLINE TArray<TSubclassOf<ULockKeyType>> GetAcquiredKeys() const { return AcquiredKeys; }

	bool AddLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) override;

	bool RemoveLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) override;

	bool RemoveAllLockKeys_Implementation() override;

	bool UseLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) override;

	bool HasLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) const override;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintGetter=GetAcquiredKeys, Category=KeyRing)
	TArray<TSubclassOf<ULockKeyType>> AcquiredKeys;
};
