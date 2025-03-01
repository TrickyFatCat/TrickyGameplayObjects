// MIT License Copyright (c) Artyom "Tricky Fat Cat" Volkov


#include "Button/ButtonInterface.h"


bool IButtonInterface::PressButton_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IButtonInterface::ReleaseButton_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IButtonInterface::DisableButton_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IButtonInterface::EnableButton_Implementation(const bool bTransitImmediately)
{
	return false;
}

bool IButtonInterface::ForceButtonState_Implementation(const EButtonState NewState, const bool bTransitImmediately)
{
	return false;
}

bool IButtonInterface::FinishButtonStateTransition_Implementation()
{
	return false;
}

bool IButtonInterface::ReverseButtonStateTransition_Implementation()
{
	return false;
}
