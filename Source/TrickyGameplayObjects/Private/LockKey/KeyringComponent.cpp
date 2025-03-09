// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "LockKey/KeyringComponent.h"
#include "GameFramework/Actor.h"

#include "LockKey/LockKeyType.h"

DEFINE_LOG_CATEGORY(LogKeyRing);

UKeyringComponent::UKeyringComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UKeyringComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UKeyringComponent::GetAcquiredLockKeys_Implementation(TArray<TSubclassOf<ULockKeyType>>& AcquiredLockKeys) const
{
	AcquiredLockKeys = AcquiredKeys;
}

bool UKeyringComponent::AddLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey)
{
	if (Execute_HasLockKey(this, LockKey))
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

bool UKeyringComponent::RemoveLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey)
{
	if (!Execute_HasLockKey(this, LockKey))
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

bool UKeyringComponent::RemoveAllLockKeys_Implementation()
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

bool UKeyringComponent::UseLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey)
{
	if (!Execute_HasLockKey(this, LockKey))
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

bool UKeyringComponent::HasLockKey_Implementation(const TSubclassOf<ULockKeyType> LockKey) const
{
	return IsValid(LockKey) && !AcquiredKeys.IsEmpty() && AcquiredKeys.Contains(LockKey);
}

#if WITH_EDITOR || !UE_BUILD_SHIPPING
void UKeyringComponent::PrintLog(const FString& Message) const
{
	const FString SourceMessage = FString::Printf(TEXT("Component: %s | Owner: %s | "),
	                                              *GetName(),
	                                              *GetOwner()->GetActorNameOrLabel());
	
	UE_LOG(LogKeyRing, Display, TEXT("%s%s"), *SourceMessage, *Message);
}
#endif
