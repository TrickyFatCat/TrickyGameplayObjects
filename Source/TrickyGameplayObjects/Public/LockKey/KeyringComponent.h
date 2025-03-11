// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "KeyringInterface.h"
#include "Components/ActorComponent.h"
#include "KeyringComponent.generated.h"

/**
 * This component provides functionality to add, remove and sue LockKeys.
 */
UCLASS(ClassGroup=(TrickyGameplayObjcets), meta=(BlueprintSpawnableComponent))
class TRICKYGAMEPLAYOBJECTS_API UKeyringComponent : public UActorComponent, public IKeyringInterface
{
	GENERATED_BODY()

public:
	UKeyringComponent();

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
	FOnAllLockKeysRemovedDynamicSignature OnAllLockKeysRemoved;

	/**
	 * Triggered when lock key is successfully used from the key ring.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnLockKeyUsedDynamicSignature OnLockKeyUsed;

	virtual void GetAcquiredLockKeys_Implementation(TArray<TSubclassOf<ULockKeyType>>& AcquiredLockKeys) const override;

	virtual bool AddLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) override;

	virtual bool RemoveLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) override;

	virtual bool RemoveAllLockKeys_Implementation() override;

	virtual bool UseLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) override;

	virtual bool HasLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) const override;

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
