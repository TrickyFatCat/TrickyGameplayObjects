// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "LockKey/KeyRingComponent.h"

#include "LockKey/LockKeyType.h"


UKeyRingComponent::UKeyRingComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UKeyRingComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UKeyRingComponent::GetAcquiredLockKeys_Implementation(TArray<TSubclassOf<ULockKeyType>>& AcquiredLockKeys) const
{
	AcquiredLockKeys = AcquiredKeys;
}

bool UKeyRingComponent::AddLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey)
{
	if (HasLockKey(LockKey))
	{
		return false;
	}

	const int32 Index = AcquiredKeys.AddUnique(LockKey);

	if (Index == INDEX_NONE)
	{
		return false;
	}

	OnLockKeyAdded.Broadcast(this, LockKey);
	return true;
}

bool UKeyRingComponent::RemoveLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey)
{
	if (!HasLockKey(LockKey))
	{
		return false;
	}

	const int32 Num = AcquiredKeys.Remove(LockKey);

	if (Num == 0)
	{
		return false;
	}

	OnLockKeyRemoved.Broadcast(this, LockKey);
	return true;
}

bool UKeyRingComponent::RemoveAllLockKeys_Implementation()
{
	if (AcquiredKeys.IsEmpty())
	{
		return false;
	}

	AcquiredKeys.Empty();
	OnAllLockKeysRemoved.Broadcast(this);
	return true;
}

bool UKeyRingComponent::UseLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey)
{
	if (!HasLockKey(LockKey))
	{
		return false;
	}

	OnLockKeyUsed.Broadcast(this, LockKey);
	return true;
}

bool UKeyRingComponent::HasLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) const
{
	return IsValid(LockKey) && !AcquiredKeys.IsEmpty() && AcquiredKeys.Contains(LockKey);
}
