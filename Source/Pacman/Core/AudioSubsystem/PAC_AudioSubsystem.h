#pragma once

#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PAC_AudioSubsystem.generated.h"

UCLASS()
class PACMAN_API UPAC_AudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

protected:
	UPROPERTY()
	TArray<UAudioComponent*> AudioComponentsAvailable;
	UPROPERTY()
	TArray<UAudioComponent*> FXAudios;
	UPROPERTY()
	UAudioComponent* BackgroundMusic;

	FDelegateHandle OnPlayBackgroundSoundHandle;
	FDelegateHandle OnPlayFXSoundHandle;
	FDelegateHandle OnPlayerDeadHandle;
	FDelegateHandle OnEndGameHandle;

	void OnPlayerDead(UPAC_Message* message);
	void OnEndGame(UPAC_Message* message);
	void OnAudioFinished(UAudioComponent* audioComponent);
	void OnPlayBackgroundSound(UPAC_Message* message);
	void OnPlayFXSound(UPAC_Message* message);

	void StopAllSounds();
	
	
}; // PAC_AudioSubsystem
