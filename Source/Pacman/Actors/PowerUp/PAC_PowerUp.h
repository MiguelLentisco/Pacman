#pragma once


#include "PaperSpriteActor.h"
#include "Pacman/Interfaces/Eatable/PAC_EatableInterface.h"
#include "PAC_PowerUp.generated.h"

class UPAC_Message;

UCLASS(Abstract)
class PACMAN_API APAC_PowerUp : public APaperSpriteActor, public IPAC_EatableInterface
{
	GENERATED_BODY()
    
public:
	APAC_PowerUp();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

	virtual void Enable() override;
	virtual void Disable() override;
	virtual void OnPlayerAte() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PowerUp, Meta = (ClampMin = 0, Units = s))
	float PowerUpTime = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PowerUp, Meta = (ClampMin = 0, Units = s))
	float EffectToggleTime = .4f;
	
	FDelegateHandle OnRoundReadyHandle;
	FTimerHandle PopEffectTimer;

	void OnRoundReady(UPAC_Message* message);
	
	virtual FName GetEatableName() const override;
	virtual UPrimitiveComponent* GetCollision() const override;

	void SetEffectTimer();
	void ToggleSpriteVisibility();
    
}; // APAC_PowerUp
