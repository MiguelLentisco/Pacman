#pragma once

#include "PaperSpriteActor.h"
#include "PAC_PointsShower.generated.h"

class UPAC_Message;
class UPaperSprite;

USTRUCT(BlueprintType)
struct PACMAN_API FPAC_SpriteByPoints
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, UPaperSprite*> SpriteByPoints;
	
}; // FPAC_SpriteByPoints

UCLASS(Abstract)
class APAC_PointsShower : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PointsShower, Meta = (ClampMin = 0, Units = s))
	float ShowTime = 1.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = PointsShower)
	TMap<FName, FPAC_SpriteByPoints> SpritePointsByItem;

	int32 LastPointsGained = 0;

	FDelegateHandle OnGainedPointsHandle;
	FDelegateHandle OnAteThingHandle;

	FTimerHandle ShowPointsTimer;

	void OnGainedPoints(UPAC_Message* message);
	void OnAteThing(UPAC_Message* message);
	void HideActor();
	
}; // APAC_PointsShower
