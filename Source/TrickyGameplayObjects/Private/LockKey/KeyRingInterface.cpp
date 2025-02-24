// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "LockKey/KeyRingInterface.h"

#include "LockKey/LockKeyType.h"


void IKeyRingInterface::GetAcquiredLockKeys_Implementation(TArray<TSubclassOf<ULockKeyType>>& AcquiredLockKeys) const
{
}

bool IKeyRingInterface::AddLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType)
{
	return IsValid(LockKeyType->StaticClass());
}

bool IKeyRingInterface::RemoveLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType)
{
	return IsValid(LockKeyType->StaticClass());
}

bool IKeyRingInterface::RemoveAllLockKeys_Implementation()
{
	return false;
}

bool IKeyRingInterface::UseLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType)
{
	return IsValid(LockKeyType->StaticClass());
}

bool IKeyRingInterface::HasLockKey_Implementation(TSubclassOf<ULockKeyType> LockKeyType) const
{
	return IsValid(LockKeyType->StaticClass());
}
