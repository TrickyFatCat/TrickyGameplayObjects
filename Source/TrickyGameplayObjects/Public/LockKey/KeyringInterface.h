// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Templates/SubclassOf.h"
#include "KeyringInterface.generated.h"

class ULockKeyType;
class UKeyringComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogKeyRing, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockKeyAddedDynamicSignature,
                                             UKeyringComponent*, Component,
                                             TSubclassOf<ULockKeyType>, LockKey);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockKeyRemovedDynamicSignature,
                                             UKeyringComponent*, Component,
                                             TSubclassOf<ULockKeyType>, LockKey);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllLockKeysRemovedDynamicSignature,
                                            UKeyringComponent*, Component);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockKeyUsedDynamicSignature,
                                             UKeyringComponent*, Component,
                                             TSubclassOf<ULockKeyType>, LockKey);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UKeyringInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for managing keyring functionality such as adding, removing and using lock keys.
 */
class TRICKYGAMEPLAYOBJECTS_API IKeyringInterface
{
	GENERATED_BODY()

public:
	/**
	 * Retrieves an array of acquired keys.
	 *
	 * @param AcquiredLockKeys Array of acquired keys.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	void GetAcquiredLockKeys(TArray<TSubclassOf<ULockKeyType>>& AcquiredLockKeys) const;

	virtual void GetAcquiredLockKeys_Implementation(TArray<TSubclassOf<ULockKeyType>>& AcquiredLockKeys) const;

	/**
	 * Adds the specified lock key type to the key ring.
	 * @warning It's impossible to add several keys of one class.
	 *
	 * @param LockKeyType A lock key class to add.
	 * @return True if the lock key was successfully added, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	bool AddLockKey(TSubclassOf<ULockKeyType> LockKeyType);

	virtual bool AddLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType);

	/**
	 * Removes a specified lock key type from the key ring.
	 *
	 * @param LockKeyType A lock key class to remove.
	 * @return True if the lock key was successfully added, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	bool RemoveLockKey(TSubclassOf<ULockKeyType> LockKeyType);

	virtual bool RemoveLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType);

	/**
	 * Removes all lock keys currently stored in the key ring.
	 * This method is intended to clear the entire key collection.
	 *
	 * @return True if all lock keys were successfully removed, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	bool RemoveAllLockKeys();

	virtual bool RemoveAllLockKeys_Implementation();

	/**
	 * Attempts to use a specified lock key type from the key ring.
	 *
	 * @param LockKeyType A lock key class to use.
	 * @return True if the key was successfully used, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	bool UseLockKey(TSubclassOf<ULockKeyType> LockKeyType);

	virtual bool UseLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType);

	/**
	 * Checks if the specified lock key type is present in the key ring.
	 *
	 * @param LockKeyType A lock key class to check.
	 * @return True if the lock key type is present in the key ring, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	bool HasLockKey(TSubclassOf<ULockKeyType> LockKeyType) const;

	virtual bool HasLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType) const;
};


