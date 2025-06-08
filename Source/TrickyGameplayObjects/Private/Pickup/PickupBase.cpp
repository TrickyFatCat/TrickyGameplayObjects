// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Pickup/PickupBase.h"

DEFINE_LOG_CATEGORY(LogPickup)

APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APickupBase::BeginDestroy()
{
	this->OnPickupActivated.Clear();

	Super::BeginDestroy();
}

bool APickupBase::ActivatePickup(AActor* Activator)
{
	if (!IsValid(Activator))
	{
#if WITH_EDITOR && !UE_BUILD_SHIPPING
		FString Name;
		GetActorName(this, Name);
		const FString Message = FString::Printf(TEXT("%s pickup activation failed. Reason: Invalid Activator"),
		                                        *Name);
		PrintError(Message);
#endif
		return false;
	}

	if (!CanBeActivated(Activator))
	{
		HandleActivationFailure(Activator);
#if WITH_EDITOR && !UE_BUILD_SHIPPING
		FString Name, ActivatorName;
		GetActorName(this, Name);
		GetActorName(Activator, ActivatorName);
		const FString Message = FString::Printf(
			TEXT("%s pickup activation failed. Reason: %s activation check failed."),
			*Name,
			*ActivatorName);
		PrintLog(Message);
#endif
		return false;
	}

	HandleActivationSuccess(Activator);
	OnPickupActivated.Broadcast(this, Activator);

#if WITH_EDITOR && !UE_BUILD_SHIPPING
	FString Name, ActivatorName;
	GetActorName(this, Name);
	GetActorName(Activator, ActivatorName);
	const FString Message = FString::Printf(
		TEXT("%s pickup successfully activated by %s."),
		*Name,
		*ActivatorName);
	PrintLog(Message);
#endif

	if (bDestroyAfterActivation)
	{
		Destroy();
	}

	return true;
}

#if WITH_EDITOR && !UE_BUILD_SHIPPING
void APickupBase::PrintLog(const FString& Message)
{
	UE_LOG(LogPickup, Log, TEXT("%s"), *Message);
}

void APickupBase::PrintWarning(const FString& Message)
{
	UE_LOG(LogPickup, Warning, TEXT("%s"), *Message);
}

void APickupBase::PrintError(const FString& Message)
{
	UE_LOG(LogPickup, Error, TEXT("%s"), *Message);
}

void APickupBase::GetActorName(const AActor* Actor, FString& OutActorName)
{
	OutActorName = IsValid(Actor) ? Actor->GetActorNameOrLabel() : TEXT("NULL");
}
#endif
