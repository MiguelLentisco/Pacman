#include "PAC_PlayerState.h"

#include "Pacman/Core/MessagingSubsystem/PAC_GameplayEvents.h"
#include "Pacman/Core/MessagingSubsystem/PAC_MessagingSubsystem.h"

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PlayerState::SetCurrentLives(int32 currentLives)
{
	CurrentLives = FMath::Max(currentLives, 0);
	OnPlayerLivesChanged.Broadcast();
	
} // SetCurrentLives

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PlayerState::SetCurrentPoints(int32 currentPoints)
{
	CurrentPoints = FMath::Max(currentPoints, 0);
	OnPlayerPointsChanged.Broadcast();
	
} // SetCurrentPoints

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	GainedPointsHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::GAINED_POINTS,
		FOnMessageSent::CreateUObject(this, &APAC_PlayerState::OnGainedPoints));
	LostLifeHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::LOST_LIFE,
		FOnMessageSent::CreateUObject(this, &APAC_PlayerState::OnLostLife));
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PlayerState::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	Super::EndPlay(endPlayReason);

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::GAINED_POINTS, GainedPointsHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::LOST_LIFE, LostLifeHandle);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PlayerState::OnGainedPoints(UPAC_Message* message)
{
	const UPAC_BasicMessage* basicMessage = Cast<UPAC_BasicMessage>(message);
	if (basicMessage == nullptr)
	{
		return;
	}

	SetCurrentPoints(FMath::Max(0, CurrentPoints + basicMessage->Int));
	
} // OnGainedPoints

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PlayerState::OnLostLife(UPAC_Message* message)
{
	SetCurrentLives(FMath::Max(0, CurrentLives - 1));
	
} // OnLostLife

// ---------------------------------------------------------------------------------------------------------------------
