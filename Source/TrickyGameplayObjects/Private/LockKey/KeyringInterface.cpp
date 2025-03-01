// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "LockKey/KeyringInterface.h"

#include "LockKey/LockKeyType.h"


void IKeyringInterface::GetAcquiredLockKeys_Implementation(TArray<TSubclassOf<ULockKeyType>>& AcquiredLockKeys) const
{
}

bool IKeyringInterface::AddLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType)
{
	return IsValid(LockKeyType->StaticClass());
}

bool IKeyringInterface::RemoveLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType)
{
	return IsValid(LockKeyType->StaticClass());
}

bool IKeyringInterface::RemoveAllLockKeys_Implementation()
{
	return false;
}

bool IKeyringInterface::UseLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType)
{
	return IsValid(LockKeyType->StaticClass());
}

bool IKeyringInterface::HasLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType) const
{
	return IsValid(LockKeyType->StaticClass());
}
