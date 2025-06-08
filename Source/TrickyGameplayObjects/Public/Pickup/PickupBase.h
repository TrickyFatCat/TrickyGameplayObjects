// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupBase.generated.h"

class USphereComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogPickup, Log, All)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPickupActivatedDyanmicSignature,
                                             APickupBase*, Pickup,
                                             AActor*, Activator);

/**
 * Represents a generic pickup actor which has the basic logic needed by any pickup.
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
	/**
	 * Called when the pickup actor was successfully activated.
	 */
	UPROPERTY(BlueprintAssignable, Category="Pickup")
	FOnPickupActivatedDyanmicSignature OnPickupActivated;

	UFUNCTION(BlueprintGetter, Category="Pickup")
	bool GetDestroyAfterActivation() const { return bDestroyAfterActivation; }

protected:
	/**
	 * Attempts to activate the pickup with the specified activator
	 *
	 * @param Activator The actor attempting to activate the pickup
	 * @return True if the pickup was successfully activated
	 */
	UFUNCTION(BlueprintCallable, Category="Pickup")
	bool ActivatePickup(AActor* Activator);

	/**
	 * Determines if the pickup can be activated by the specified actor
	 *
	 * @param Activator The actor attempting to activate the pickup
	 * @return True if the pickup can be activated
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	bool CanBeActivated(AActor* Activator);

	virtual bool CanBeActivated_Implementation(AActor* Activator)
	{
		return true;
	}

	/**
	 * Handles the logic to be executed when the pickup is successfully activated.
	 *
	 * @param Activator The actor attempting to activate the pickup
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	void HandleActivationSuccess(AActor* Activator);

	virtual void HandleActivationSuccess_Implementation(AActor* Activator)
	{
	}

	/**
	 * Handles the logic to be executed when the activation of the pickup fails
	 *
	 * @param Activator The actor that attempted to activate the pickup
	 */
	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	void HandleActivationFailure(AActor* Activator);

	virtual void HandleActivationFailure_Implementation(AActor* Activator)
	{
	}

private:
	/**
	 * Determines if the actor will be destroyed after successful activation
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetDestroyAfterActivation, Category="Pickup")
	bool bDestroyAfterActivation = true;

#if WITH_EDITOR && !UE_BUILD_SHIPPING
	static void PrintLog(const FString& Message);

	static void PrintWarning(const FString& Message);

	static void PrintError(const FString& Message);

	static void GetActorName(const AActor* Actor, FString& OutActorName);
#endif
};
