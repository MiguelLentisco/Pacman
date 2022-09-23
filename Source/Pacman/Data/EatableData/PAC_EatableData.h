#pragma once

#include "Engine/DataTable.h"
#include "PAC_EatableData.generated.h"

USTRUCT(BlueprintType)
struct PACMAN_API FPAC_EatableData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	int32 Points = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> Sound = nullptr;
	
}; // FPAC_EatableData
