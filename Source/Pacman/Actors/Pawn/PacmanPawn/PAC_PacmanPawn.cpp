#include "PAC_PacmanPawn.h"

#include "Pacman/Interfaces/Eatable/PAC_EatableInterface.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Pacman/Core/MessagingSubsystem/PAC_GameplayEvents.h"
#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "Pacman/Core/MessagingSubsystem/PAC_MessagingSubsystem.h"
#include "Pacman/Settings/PAC_PacmanSettings.h"

// ---------------------------------------------------------------------------------------------------------------------

APAC_PacmanPawn::APAC_PacmanPawn() : Super()
{
	InteractableCollision = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("InteractableCollision"));
	InteractableCollision->SetupAttachment(RootComponent);
	InteractableCollision->OnComponentBeginOverlap.AddDynamic(this, &APAC_PacmanPawn::OnInteractableOverlapped);

	PaperFlipbook->SetCollisionObjectType(GetDefault<UPAC_PacmanSettings>()->PawnSpawnedChannel);
	
} // APAC_PacmanPawn

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PacmanPawn::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
	
	DisableInput(Cast<APlayerController>(newController));
	
} // PossessedBy

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PacmanPawn::OnRoundReady(UPAC_Message* message)
{
	Super::OnRoundReady(message);

	EnableInput(Cast<APlayerController>(GetController()));
	
} // OnRoundReady

// ---------------------------------------------------------------------------------------------------------------------

APlayerController* APAC_PacmanPawn::GetPlayerController() const
{
	return Cast<APlayerController>(GetController());
	
} // GetPlayerController

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PacmanPawn::AfterPlayerDeath()
{
	Super::AfterPlayerDeath();

	PaperFlipbook->SetComponentTickEnabled(true);
	PaperFlipbook->SetFlipbook(DeathAnimation);
	PaperFlipbook->SetLooping(false);

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->SendEvent(PAC_GameplayEvents::PLAY_BACKGROUND_SOUND,
		UPAC_BasicMessage::CreateBasicMessage(this, DeathSound));
	
	PaperFlipbook->OnFinishedPlaying.AddDynamic(this, &APAC_PacmanPawn::OnDeathAnimationEnded);
	
} // AfterPlayerDeath

// ---------------------------------------------------------------------------------------------------------------------


void APAC_PacmanPawn::OnSetPausePawnsNative(bool bPause)
{
	Super::OnSetPausePawnsNative(bPause);
	
	SetActorHiddenInGame(bPause);
	
	APlayerController* playerController = GetPlayerController();
	bPause ? DisableInput(playerController) : EnableInput(playerController);
	
} // OnSetPausePawns

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PacmanPawn::OnInteractableOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (!UKismetSystemLibrary::DoesImplementInterface(otherActor, UPAC_EatableInterface::StaticClass()))
	{
		return;
	}

	IPAC_EatableInterface* eatable = Cast<IPAC_EatableInterface>(otherActor);
	eatable->OnPlayerAte();
	
} // OnInteractableOverlapped

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PacmanPawn::OnDeathAnimationEnded()
{
	GetWorldTimerManager().SetTimer(NotifyGameStateTimer, FTimerDelegate::CreateWeakLambda(this, [this]()
	{
		UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
		messagingSubsystem->SendEvent(PAC_GameplayEvents::LOST_LIFE, nullptr);
	}), NotifyGameStateTime, false);
	PaperFlipbook->OnFinishedPlaying.RemoveAll(this);
	
} // OnDeathAnimationEnded

// ---------------------------------------------------------------------------------------------------------------------
