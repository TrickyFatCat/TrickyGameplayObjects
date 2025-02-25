// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "KeyRingInterface.h"
#include "Components/ActorComponent.h"
#include "KeyRingComponent.generated.h"

/**
 * This component provides functionality to add, remove and sue LockKeys.
 */
UCLASS(ClassGroup=(TrickyGameplayObjcets), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API UKeyRingComponent : public UActorComponent, public IKeyRingInterface
{
	GENERATED_BODY()

public:
	UKeyRingComponent();

protected:
	virtual void InitializeComponent() override;

public:
	/**
	 * Triggered when a new lock key is successfully added to the key ring.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnLockKeyAddedDynamicSignature OnLockKeyAdded;

	/**
	 * Triggered whenever a lock key is removed from the key ring.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnLockKeyRemovedDynamicSignature OnLockKeyRemoved;

	/**
	 * Triggered when all lock keys have been removed from the key ring.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnAllLockKeysRemovedDynmaicSignature OnAllLockKeysRemoved;

	/**
	 * Triggered when lock key is successfully used from the key ring.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnLockKeyUsedDynamicSignature OnLockKeyUsed;

	void GetAcquiredLockKeys_Implementation(TArray<TSubclassOf<ULockKeyType>>& AcquiredLockKeys) const override;

	bool AddLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) override;

	bool RemoveLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) override;

	bool RemoveAllLockKeys_Implementation() override;

	bool UseLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) override;

	bool HasLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) const override;

private:
	/**
	 * Stores lock keys that have been acquired.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category=KeyRing, meta=(AllowPrivateAccess))
	TArray<TSubclassOf<ULockKeyType>> AcquiredKeys;

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	void PrintLog(const FString& Message) const;
#endif
};
