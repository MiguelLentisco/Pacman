#pragma once

#include "PAC_PacmanSettings.generated.h"

class UDataTable;

UCLASS(Config = Game, DefaultConfig, DisplayName = "Pacman Settings")
class PACMAN_API UPAC_PacmanSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = Collision)
	TEnumAsByte<ECollisionChannel> PawnSpawnedChannel = ECC_GameTraceChannel1;
	
	UPROPERTY(Config, EditAnywhere, Category = Points, Meta = (RowType = "PAC_EatableData"))
	TSoftObjectPtr<UDataTable> EatableDataTable = nullptr;

	UPROPERTY(Config, EditAnywhere, Category = Sound)
	TSoftObjectPtr<USoundConcurrency> SoundConcurrencySettings = nullptr;

	UPROPERTY(Config, EditAnywhere, Category = Sound)
	TSoftObjectPtr<USoundBase> AlarmBackgroundMusic = nullptr;
	UPROPERTY(Config, EditAnywhere, Category = Sound)
	TSoftObjectPtr<USoundBase> PowerUpBackgroundMusic = nullptr;
	
}; // UPAC_PacmanSettings
