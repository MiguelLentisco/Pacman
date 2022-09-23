#pragma once

#include "Engine/TriggerBox.h"
#include "PAC_Teleport.generated.h"

UCLASS(Abstract)
class PACMAN_API APAC_Teleport : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	APAC_Teleport();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Teleport)
	AActor* TeleportPoint = nullptr;

	UFUNCTION()
	void OnPawnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
		UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	
}; // APAC_Teleport
