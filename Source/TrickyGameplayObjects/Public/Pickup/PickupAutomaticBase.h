// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "PickupAutomaticBase.generated.h"

/**
 * Represents a pickup actor which activates its logic automatically on activation trigger overlap
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class TRICKYGAMEPLAYOBJECTS_API APickupAutomaticBase : public APickupBase
{
	GENERATED_BODY()

public:
	APickupAutomaticBase();

protected:
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Components")
	USphereComponent* ActivationTrigger = nullptr;

	UFUNCTION()
	void HandleActivationTriggerOverlap(UPrimitiveComponent* OverlappedComponent,
	                                    AActor* OtherActor,
	                                    UPrimitiveComponent* OtherComp,
	                                    int32 OtherBodyIndex,
	                                    bool bFromSweep,
	                                    const FHitResult& SweepResult);
};
