// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Pickup/PickupAutomaticBase.h"

#include "Engine/World.h"
#include "Components/SphereComponent.h"


APickupAutomaticBase::APickupAutomaticBase()
{
	PrimaryActorTick.bCanEverTick = true;

	ActivationTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("ActivationTrigger"));
	ActivationTrigger->SetupAttachment(GetRootComponent());
	ActivationTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ActivationTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	ActivationTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void APickupAutomaticBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!GetWorld()->IsGameWorld())
	{
		return;
	}

	ActivationTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupAutomaticBase::HandleActivationTriggerOverlap);
}

void APickupAutomaticBase::HandleActivationTriggerOverlap(UPrimitiveComponent* OverlappedComponent,
                                                      AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp,
                                                      int32 OtherBodyIndex,
                                                      bool bFromSweep,
                                                      const FHitResult& SweepResult)
{
	ActivatePickup(OtherActor);
}
