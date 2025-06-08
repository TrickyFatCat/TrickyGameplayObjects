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

bool APickupBase::ActivatePickup(AActor* Activator)
{
	if (!IsValid(Activator))
	{
		return false;
	}

	if (!CanBeActivated(Activator))
	{
		HandleActivationFailure(Activator);
		return false;
	}

	HandleActivationSuccess(Activator);
	OnPickupActivated.Broadcast(this, Activator);

	if (bDestroyAfterActivation)
	{
		Destroy();
	}

	return true;
}
