#include "PAC_AudioSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Pacman/Core/MessagingSubsystem/PAC_GameplayEvents.h"
#include "Pacman/Core/MessagingSubsystem/PAC_MessagingSubsystem.h"
#include "Components/AudioComponent.h"
#include "Pacman/Settings/PAC_PacmanSettings.h"

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_AudioSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
    collection.InitializeDependency(UPAC_MessagingSubsystem::StaticClass());
	
    UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
    OnPlayFXSoundHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::PLAY_FX_SOUND,
    	FOnMessageSent::CreateUObject(this, &UPAC_AudioSubsystem::OnPlayFXSound));
	OnPlayBackgroundSoundHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::PLAY_BACKGROUND_SOUND,
		FOnMessageSent::CreateUObject(this, &UPAC_AudioSubsystem::OnPlayBackgroundSound));
	OnEndGameHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::END_GAME,
			FOnMessageSent::CreateUObject(this, &UPAC_AudioSubsystem::OnEndGame));
	OnPlayerDeadHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::PLAYER_DEAD,
		FOnMessageSent::CreateUObject(this, &UPAC_AudioSubsystem::OnPlayerDead));

} // Initialize

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_AudioSubsystem::Deinitialize()
{
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::PLAY_FX_SOUND, OnPlayFXSoundHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::PLAY_BACKGROUND_SOUND, OnPlayBackgroundSoundHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::END_GAME, OnEndGameHandle);
	
	Super::Deinitialize();
	
} // Deinitialize

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_AudioSubsystem::OnPlayerDead(UPAC_Message* message)
{
	StopAllSounds();
	
} // OnPlayerDead

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_AudioSubsystem::OnEndGame(UPAC_Message* message)
{
	StopAllSounds();
	
} // OnEndGame

// ---------------------------------------------------------------------------------------------------------------------


void UPAC_AudioSubsystem::OnAudioFinished(UAudioComponent* audioComponent)
{
	if (audioComponent == nullptr)
	{
		return;
	}

	FXAudios.Remove(audioComponent);
	AudioComponentsAvailable.AddUnique(audioComponent);
	
} // OnAudioFinished

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_AudioSubsystem::OnPlayBackgroundSound(UPAC_Message* message)
{
	const UPAC_BasicMessage* basicMessage = Cast<UPAC_BasicMessage>(message);
	if (message == nullptr)
	{
		return;
	}

	USoundBase* sound = Cast<USoundBase>(basicMessage->Object);
	if (sound == nullptr)
	{
		return;
	}

	if (BackgroundMusic != nullptr)
	{
		BackgroundMusic->SetSound(sound);
	}
	else
	{
		BackgroundMusic = UGameplayStatics::CreateSound2D(this, sound, 1.f, 1.f, 0.f,
			GetDefault<UPAC_PacmanSettings>()->SoundConcurrencySettings.LoadSynchronous());
		BackgroundMusic->bAutoDestroy = false;
	}

	BackgroundMusic->Play();
	
} // OnPlayBackgroundSound

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_AudioSubsystem::OnPlayFXSound(UPAC_Message* message)
{
	const UPAC_BasicMessage* basicMessage = Cast<UPAC_BasicMessage>(message);
	if (message == nullptr)
	{
		return;
	}

	USoundBase* sound = Cast<USoundBase>(basicMessage->Object);
	if (sound == nullptr)
	{
		return;
	}

	UAudioComponent* audioComp;
	if (AudioComponentsAvailable.Num() == 0)
	{
		audioComp = UGameplayStatics::CreateSound2D(this, sound, 1.f, 1.f, 0.f,
			GetDefault<UPAC_PacmanSettings>()->SoundConcurrencySettings.LoadSynchronous());
		if (audioComp == nullptr)
		{
			return;
		}

		audioComp->bAutoDestroy = false;
		audioComp->OnAudioFinishedNative.AddUObject(this, &UPAC_AudioSubsystem::OnAudioFinished);
	}
	else
	{
		audioComp = AudioComponentsAvailable[0];
		AudioComponentsAvailable.RemoveAt(0);

		if (audioComp == nullptr)
		{
			return;
		}
	}
	
	FXAudios.Add(audioComp);
	audioComp->Play();
	
} // OnPlayFXSound

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_AudioSubsystem::StopAllSounds()
{
	if (BackgroundMusic != nullptr)
	{
		BackgroundMusic->Stop();
	}

	for (UAudioComponent* audioComponent : FXAudios)
	{
		if (audioComponent != nullptr)
		{
			audioComponent->Stop();
			AudioComponentsAvailable.AddUnique(audioComponent);
		}
	}
	FXAudios.Reset();
	
} // StopAllSounds

// ---------------------------------------------------------------------------------------------------------------------
