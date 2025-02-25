// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "LockKey/KeyRingComponent.h"
#include "GameFramework/Actor.h"

#include "LockKey/LockKeyType.h"

DEFINE_LOG_CATEGORY(LogKeyRing);

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

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	const FString LogMessage = FString::Printf(TEXT("New LockKey Added: %s | Index: %d"), *LockKey->GetName(), Index);
	PrintLog(LogMessage);
#endif
	
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

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	const FString LogMessage = FString::Printf(TEXT("Removed LockKey: %s"), *LockKey->GetName());
	PrintLog(LogMessage);
#endif
	
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
	
#if WITH_EDITOR || !UE_BUILD_SHIPPING
	PrintLog("Removed all acquired LockKeys");
#endif
	
	OnAllLockKeysRemoved.Broadcast(this);
	return true;
}

bool UKeyRingComponent::UseLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey)
{
	if (!HasLockKey(LockKey))
	{
		return false;
	}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
	const FString LogMessage = FString::Printf(TEXT("LockKey Used: %s"), *LockKey->GetName());
	PrintLog(LogMessage);
#endif
	
	OnLockKeyUsed.Broadcast(this, LockKey);
	return true;
}

bool UKeyRingComponent::HasLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) const
{
	return IsValid(LockKey) && !AcquiredKeys.IsEmpty() && AcquiredKeys.Contains(LockKey);
}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
void UKeyRingComponent::PrintLog(const FString& Message) const
{
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *GetOwner()->GetActorNameOrLabel());
	
	UE_LOG(LogKeyRing, Display, TEXT("%s%s"), *SourceMessage, *Message);
}
#endif
