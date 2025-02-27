// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Door/DoorInterface.h"

bool IDoorInterface::OpenDoor_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IDoorInterface::CloseDoor_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IDoorInterface::LockDoor_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IDoorInterface::UnlockDoor_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IDoorInterface::DisableDoor_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IDoorInterface::EnableDoor_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IDoorInterface::ForceDoorState_Implementation(const EDoorState NewState, const bool bTransitImmediately)
{
	return false;
}

bool IDoorInterface::FinishDoorStateTransition_Implementation()
{
	return false;
}

bool IDoorInterface::ReverseDoorStateTransition_Implementation()
{
	return false;
}
