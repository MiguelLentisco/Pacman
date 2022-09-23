#pragma once

#include "PAC_Direction.generated.h"

UENUM(BlueprintType)
enum class EPAC_Direction : uint8
{
	Up,
	Down,
	Left,
	Right,
	Num		UMETA(Hidden),
	
}; // EPAC_AnimationDirection