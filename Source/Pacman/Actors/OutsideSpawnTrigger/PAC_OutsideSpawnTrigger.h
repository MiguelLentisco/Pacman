#pragma once

#include "Engine/TriggerBox.h"
#include "PAC_OutsideSpawnTrigger.generated.h"

UCLASS(Abstract)
class PACMAN_API APAC_OutsideSpawnTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	APAC_OutsideSpawnTrigger();

protected:
	UFUNCTION()
	void OnGhostEntered(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
		UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	
}; // APAC_OutsideSpawnTrigger
