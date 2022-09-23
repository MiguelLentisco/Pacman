#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PacmanGameModeBase.generated.h"

UCLASS()
class PACMAN_API APacmanGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void StartPlay() override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pacman, Meta = (ClampMin = 0))
	int32 InitialLives = 5;
	
	void DisableToneMapper() const;
	
}; // APacmanGameModeBase
