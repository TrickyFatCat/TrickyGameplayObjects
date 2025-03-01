// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Chest/ChestInterface.h"


bool IChestInterface::OpenChest_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IChestInterface::CloseChest_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IChestInterface::LockChest_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IChestInterface::UnlockChest_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IChestInterface::DisableChest_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IChestInterface::EnableChest_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IChestInterface::ForceChestState_Implementation(const EChestState NewState, const bool bTransitImmediately)
{
	return false;
}

bool IChestInterface::FinishChestStateTransition_Implementation()
{
	return false;
}

bool IChestInterface::ReverseChestStateTransition_Implementation()
{
	return false;
}
