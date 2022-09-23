#pragma once

#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "PAC_PowerUpSubsystem.generated.h"

UCLASS()
class UPAC_PowerUpSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

protected:
	FTimerHandle PowerUpTimer;
	
	FDelegateHandle OnRoundReadyHandle;
	FDelegateHandle OnPowerUpGainedHandle;
	FDelegateHandle OnEndGameHandle;
	FDelegateHandle OnSetPausePawnsHandle;
	FDelegateHandle OnPlayerDeathHandle;

	void OnRoundReady(UPAC_Message* message);
	void OnPowerUpGained(UPAC_Message* message);
	void OnEndGame(UPAC_Message* message);
	void OnSetPausePawns(UPAC_Message* message);
	void OnPlayerDeath(UPAC_Message* message);

	void OnPowerUpEnded();
	
}; // UPAC_PowerUpSubsystem
