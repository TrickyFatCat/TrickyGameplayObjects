// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Templates/SubclassOf.h"
#include "KeyRingInterface.generated.h"

class ULockKeyType;
class UKeyRingComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockKeyAddedDynamicSignature,
                                             UKeyRingComponent*, Component,
                                             TSubclassOf<ULockKeyType>, LockKey);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockKeyRemovedDynamicSignature,
                                             UKeyRingComponent*, Component,
                                             TSubclassOf<ULockKeyType>, LockKey);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllLockKeysRemovedDynmaicSignature,
                                            UKeyRingComponent*, Component);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLockKeyUsedDynamicSignature,
                                             UKeyRingComponent*, Component,
                                             TSubclassOf<ULockKeyType>, LockKey);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UKeyRingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TRICKYGAMEPLAYOBJECTS_API IKeyRingInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	bool AddLockKey(TSubclassOf<ULockKeyType> LockKeyType);

	virtual bool AddLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	bool RemoveLockKey(TSubclassOf<ULockKeyType> LockKeyType);

	virtual bool RemoveLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	bool RemoveAllLockKeys();

	virtual bool RemoveAllLockKeys_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	bool UseLockKey(TSubclassOf<ULockKeyType> LockKeyType);

	virtual bool UseLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = KeyRing)
	bool HasLockKey(TSubclassOf<ULockKeyType> LockKeyType) const;

	virtual bool HasLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType) const;
};


