#include "PAC_PowerUpSubsystem.h"

#include "Pacman/Core/MessagingSubsystem/PAC_GameplayEvents.h"
#include "Pacman/Core/MessagingSubsystem/PAC_MessagingSubsystem.h"
#include "Pacman/Settings/PAC_PacmanSettings.h"

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PowerUpSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	collection.InitializeDependency(UPAC_MessagingSubsystem::StaticClass());

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	OnRoundReadyHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::ROUND_READY,
		FOnMessageSent::CreateUObject(this, &UPAC_PowerUpSubsystem::OnRoundReady));
	OnPowerUpGainedHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::POWER_UP_OBTAINED,
		FOnMessageSent::CreateUObject(this, &UPAC_PowerUpSubsystem::OnPowerUpGained));
	OnEndGameHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::END_GAME,
		FOnMessageSent::CreateUObject(this, &UPAC_PowerUpSubsystem::OnEndGame));
	OnSetPausePawnsHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::SET_PAUSE_PAWNS,
		FOnMessageSent::CreateUObject(this, &UPAC_PowerUpSubsystem::OnSetPausePawns));
	OnPlayerDeathHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::PLAYER_DEAD,
		FOnMessageSent::CreateUObject(this, &UPAC_PowerUpSubsystem::OnPlayerDeath));
		
} // Initialize

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PowerUpSubsystem::Deinitialize()
{
	GetWorld()->GetTimerManager().ClearTimer(PowerUpTimer);
	
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::ROUND_READY, OnRoundReadyHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::POWER_UP_OBTAINED, OnPowerUpGainedHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::END_GAME, OnEndGameHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::SET_PAUSE_PAWNS, OnSetPausePawnsHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::PLAYER_DEAD, OnPlayerDeathHandle);
	
	Super::Deinitialize();
	
} // Deinitialize

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PowerUpSubsystem::OnRoundReady(UPAC_Message* message)
{
	
} // OnRoundReady

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PowerUpSubsystem::OnPowerUpGained(UPAC_Message* message)
{
	const UPAC_BasicMessage* basicMessage = Cast<UPAC_BasicMessage>(message);
	if (basicMessage == nullptr)
	{
		return;
	}
	
	FTimerManager& timerMgr = GetWorld()->GetTimerManager();
	if (!timerMgr.IsTimerActive(PowerUpTimer))
	{
		UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
		USoundBase* backgroundSound = GetDefault<UPAC_PacmanSettings>()->PowerUpBackgroundMusic.LoadSynchronous();
		messagingSubsystem->SendEvent(PAC_GameplayEvents::PLAY_BACKGROUND_SOUND,
			UPAC_BasicMessage::CreateBasicMessage(this, backgroundSound));
	}

	timerMgr.SetTimer(PowerUpTimer, this, &UPAC_PowerUpSubsystem::OnPowerUpEnded, basicMessage->Float, false);
	
} // OnPowerUpGained

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PowerUpSubsystem::OnEndGame(UPAC_Message* message)
{
	GetWorld()->GetTimerManager().ClearTimer(PowerUpTimer);
	
} // OnEndGame

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PowerUpSubsystem::OnSetPausePawns(UPAC_Message* message)
{
	const UPAC_BasicMessage* basicMessage = Cast<UPAC_BasicMessage>(message);
	if (basicMessage == nullptr)
	{
		return;
	}

	FTimerManager& timerMgr = GetWorld()->GetTimerManager();
	basicMessage->Bool ? timerMgr.PauseTimer(PowerUpTimer) : timerMgr.UnPauseTimer(PowerUpTimer);
	
} // OnSetPausePawns

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PowerUpSubsystem::OnPlayerDeath(UPAC_Message* message)
{
	GetWorld()->GetTimerManager().ClearTimer(PowerUpTimer);
	
} // OnPlayerDeath

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PowerUpSubsystem::OnPowerUpEnded()
{
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->SendEvent(PAC_GameplayEvents::POWER_UP_FINISHED, nullptr);

	USoundBase* backgroundSound = GetDefault<UPAC_PacmanSettings>()->AlarmBackgroundMusic.LoadSynchronous();
	messagingSubsystem->SendEvent(PAC_GameplayEvents::PLAY_BACKGROUND_SOUND,
		UPAC_BasicMessage::CreateBasicMessage(this, backgroundSound));
	
} // OnPowerUpEnded

// ---------------------------------------------------------------------------------------------------------------------
