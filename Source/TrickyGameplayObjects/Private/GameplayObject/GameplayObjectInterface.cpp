// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "GameplayObject/GameplayObjectInterface.h"


bool IGameplayObjectInterface::ActivateGameplayObject_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IGameplayObjectInterface::DeactivateGameplayObject_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IGameplayObjectInterface::DisableGameplayObject_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IGameplayObjectInterface::EnableGameplayObject_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IGameplayObjectInterface::ForceGameplayObjectState_Implementation(const EGameplayObjectState NewState,
                                                                       const bool bTransitImmediately)
{
	return false;
}

bool IGameplayObjectInterface::FinishGameplayObjetStateTransition_Implementation()
{
	return false;
}

bool IGameplayObjectInterface::ReverseGameplayObjectStateTransition_Implementation()
{
	return false;
}
