// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPickupActivatedDyanmicSignature,
                                             APickupBase*, Pickup,
                                             AActor*, Activator);

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TRICKYGAMEPLAYOBJECTS_API APickupBase : public AActor
{
	GENERATED_BODY()

public:
	APickupBase();

protected:
	virtual void BeginDestroy() override;

public:
	UPROPERTY(BlueprintAssignable, Category="Pickup")
	FOnPickupActivatedDyanmicSignature OnPickupActivated;

	UFUNCTION(BlueprintGetter, Category="Pickup")
	bool GetDestroyAfterActivation() const { return bDestroyAfterActivation; }

protected:
	UFUNCTION(BlueprintCallable, Category="Pickup")
	void ActivatePickup(AActor* Activator);
	
	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	bool CanBeActivated(AActor* Activator);

	virtual bool CanBeActivated_Implementation(AActor* Activator)
	{
		return true;
	}

	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	void HandleActivationSuccess(AActor* Activator);

	virtual void HandleActivationSuccess_Implementation(AActor* Activator)
	{
	}

	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	void HandleActivationFailure(AActor* Activator);

	virtual void HandleActivationFailure_Implementation(AActor* Activator)
	{
	}

private:
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetDestroyAfterActivation, Category="Pickup")
	bool bDestroyAfterActivation = true;
};
