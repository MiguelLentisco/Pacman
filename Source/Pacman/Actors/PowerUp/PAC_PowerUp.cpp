#include "PAC_PowerUp.h"

#include "Pacman/Core/MessagingSubsystem/PAC_MessagingSubsystem.h"
#include "Pacman/Core/MessagingSubsystem/PAC_GameplayEvents.h"
#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "PaperSpriteComponent.h"

// ---------------------------------------------------------------------------------------------------------------------

APAC_PowerUp::APAC_PowerUp() : Super()
{
	PrimaryActorTick.bCanEverTick = false;
	
} // APAC_PowerUp

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PowerUp::BeginPlay()
{
	Super::BeginPlay();

	Enable();
	GetWorldTimerManager().ClearTimer(PopEffectTimer);

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	OnRoundReadyHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::ROUND_READY,
		FOnMessageSent::CreateUObject(this, &APAC_PowerUp::OnRoundReady));
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PowerUp::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	 messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::ROUND_READY, OnRoundReadyHandle);

	Super::EndPlay(endPlayReason);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PowerUp::Enable()
{
	IPAC_EatableInterface::Enable();
	SetEffectTimer();
	
} // Enable

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PowerUp::Disable()
{
	GetWorldTimerManager().ClearTimer(PopEffectTimer);
	IPAC_EatableInterface::Disable();
	
} // Disable

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PowerUp::OnPlayerAte()
{
	IPAC_EatableInterface::OnPlayerAte();
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->SendEvent(PAC_GameplayEvents::POWER_UP_OBTAINED,
		UPAC_BasicMessage::CreateBasicMessage(this, PowerUpTime));
	
} // OnPlayerAte

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PowerUp::OnRoundReady(UPAC_Message* message)
{
	SetEffectTimer();
	
} // OnRoundReady

// ---------------------------------------------------------------------------------------------------------------------

FName APAC_PowerUp::GetEatableName() const
{
	return TEXT("PowerUp");
	
} // GetEatableName

// ---------------------------------------------------------------------------------------------------------------------

UPrimitiveComponent* APAC_PowerUp::GetCollision() const
{
	return GetRenderComponent();
	
} // GetCollision

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PowerUp::SetEffectTimer()
{
	GetWorldTimerManager().SetTimer(PopEffectTimer, this, &APAC_PowerUp::ToggleSpriteVisibility, EffectToggleTime, true);
	
} // SetEffectTimer

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PowerUp::ToggleSpriteVisibility()
{
	SetActorHiddenInGame(!IsHidden());
	
} // ToggleSpriteVisibility

// ---------------------------------------------------------------------------------------------------------------------
