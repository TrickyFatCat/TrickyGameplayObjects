// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Lock/LockInterface.h"


// Add default functionality here for any ILockInterface functions that are not pure virtual.
bool ILockInterface::Lock_Implementation(AActor* OtherActor, const bool bTransitImmediately)
{
	return false;
}

bool ILockInterface::Unlock_Implementation(AActor* OtherActor, const bool bTransitImmediately)
{
	return false;
}

bool ILockInterface::Disable_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool ILockInterface::Enable_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool ILockInterface::ForceState_Implementation(const ELockState NewState, const bool bTransitImmediately)
{
	return false;
}
