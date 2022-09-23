#include "PAC_PacmanController.h"

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PacmanController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const FName X_AXIS_BIND = TEXT("XAxis");
	const FName Y_AXIS_BIND = TEXT("YAxis");
	
	InputComponent->BindAxis(X_AXIS_BIND, this, &APAC_PacmanController::OnXAxis);
	InputComponent->BindAxis(Y_AXIS_BIND, this, &APAC_PacmanController::OnYAxis);
	
} // SetupInputComponent

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PacmanController::OnXAxis(float value)
{
	if (FMath::IsNearlyZero(value))
	{
		return;
	}
	
	GetPawn()->ConsumeMovementInputVector();
	GetPawn()->AddMovementInput(FMath::Sign(value) * FVector::ForwardVector);
	
} // OnXAxis

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PacmanController::OnYAxis(float value)
{
	if (FMath::IsNearlyZero(value))
	{
		return;
	}
	
	GetPawn()->ConsumeMovementInputVector();
	GetPawn()->AddMovementInput(FMath::Sign(value) * FVector::UpVector);
	
} // OnYAxis

// ---------------------------------------------------------------------------------------------------------------------
