// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Pickup/PickupBase.h"


APickupBase::APickupBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APickupBase::BeginDestroy()
{
	this->OnPickupActivated.Clear();
	
	Super::BeginDestroy();
}

void APickupBase::ActivatePickup(AActor* Activator)
{
	if (!IsValid(Activator))
	{
		return;
	}

	if (!CanBeActivated(Activator))
	{
		HandleActivationFailure(Activator);
		return;
	}

	HandleActivationSuccess(Activator);
	OnPickupActivated.Broadcast(this, Activator);

	if (bDestroyAfterActivation)
	{
		Destroy();
	}
}
