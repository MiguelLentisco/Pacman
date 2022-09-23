#include "PAC_GameState.h"

#include "Pacman/Core/MessagingSubsystem/PAC_GameplayEvents.h"
#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "Pacman/Core/MessagingSubsystem/PAC_MessagingSubsystem.h"
#include "Pacman/Actors/Dot/PAC_Dot.h"
#include "Pacman/Actors/PowerUp/PAC_PowerUp.h"
#include "Pacman/Settings/PAC_PacmanSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Pacman/Core/PlayerState/PAC_PlayerState.h"

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GameState::RoundReady()
{
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->SendEvent(PAC_GameplayEvents::ROUND_READY, nullptr);
	
	USoundBase* backgroundSound = GetDefault<UPAC_PacmanSettings>()->AlarmBackgroundMusic.LoadSynchronous();
	messagingSubsystem->SendEvent(PAC_GameplayEvents::PLAY_BACKGROUND_SOUND,
		UPAC_BasicMessage::CreateBasicMessage(this, backgroundSound));
	
} // RoundReady

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GameState::HandleBeginPlay()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(this, APAC_Dot::StaticClass(), actors);
	NumDots = actors.Num();

	UGameplayStatics::GetAllActorsOfClass(this, APAC_PowerUp::StaticClass(), actors);
	NumPowerUps = actors.Num();
	
	Super::HandleBeginPlay();
	
} // HandleBeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GameState::BeginPlay()
{
	Super::BeginPlay();

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	OnAteThingHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::ATE_THING,
		FOnMessageSent::CreateUObject(this, &APAC_GameState::OnAteThing));

	const APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerState = playerController->GetPlayerState<APAC_PlayerState>();
	PlayerState->OnPlayerLivesChanged.AddDynamic(this, &APAC_GameState::CheckLives);
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GameState::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::ATE_THING, OnAteThingHandle);

	Super::EndPlay(endPlayReason);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GameState::OnAteThing(UPAC_Message* message)
{
	const UPAC_BasicMessage* basicMessage = Cast<UPAC_BasicMessage>(message);
	if (basicMessage == nullptr)
	{
		return;
	}

	const UObject* eatable = basicMessage->Object;
	if (eatable == nullptr || !eatable->Implements<UPAC_EatableInterface>())
	{
		return;
	}
	
	const FName name = Cast<IPAC_EatableInterface>(eatable)->GetEatableName();
	if (name == TEXT("Dot"))
	{
		NumDots = FMath::Max(0, NumDots - 1);
	}
	else if (name == TEXT("PowerUp"))
	{
		NumPowerUps = FMath::Max(0, NumPowerUps - 1);
	}
	CheckFinished();
	
} // OnAteThing

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GameState::CheckFinished()
{
	if (NumDots > 0 || NumPowerUps > 0)
	{
		return;
	}

	EndGame(true);
	
} // CheckFinished

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GameState::EndGame(bool bWin)
{
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->SendEvent(PAC_GameplayEvents::END_GAME, UPAC_BasicMessage::CreateBasicMessage(this, bWin));
	GameFinished(bWin);
	
} // EndGame

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GameState::CheckLives()
{
	if (PlayerState->GetCurrentLives() == 0)
	{
		EndGame(false);
		return;
	}

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->SendEvent(PAC_GameplayEvents::RESET_SPAWN, nullptr);
	ShowResetAnimation();
	
} // CheckLives

// ---------------------------------------------------------------------------------------------------------------------
