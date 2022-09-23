#include "PAC_Dot.h"

#include "PaperSpriteComponent.h"

// ---------------------------------------------------------------------------------------------------------------------

APAC_Dot::APAC_Dot() : Super()
{
	PrimaryActorTick.bCanEverTick = false;
	
} // APAC_Dot

// ---------------------------------------------------------------------------------------------------------------------

FName APAC_Dot::GetEatableName() const
{
	return TEXT("Dot");
	
} // GetEatableName

// ---------------------------------------------------------------------------------------------------------------------

UPrimitiveComponent* APAC_Dot::GetCollision() const
{
	return GetRenderComponent();
	
} // GetCollision

// ---------------------------------------------------------------------------------------------------------------------
