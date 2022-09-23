#include "PAC_BasePawn.h"

#include "PAC_PawnMovementComponent.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pacman/Core/MessagingSubsystem/PAC_GameplayEvents.h"
#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "Pacman/Core/MessagingSubsystem/PAC_MessagingSubsystem.h"

// ---------------------------------------------------------------------------------------------------------------------

APAC_BasePawn::APAC_BasePawn() : Super()
{
	PrimaryActorTick.bStartWithTickEnabled = false;
	
	PaperFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("PaperFlipbook"));
	SetRootComponent(PaperFlipbook);
	PaperFlipbook->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	MovementComponent = CreateDefaultSubobject<UPAC_PawnMovementComponent>(TEXT("PawnMovement"));
	MovementComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	
} // APAC_BasePawn

// ---------------------------------------------------------------------------------------------------------------------

UPawnMovementComponent* APAC_BasePawn::GetMovementComponent() const
{
	return MovementComponent;
	
} // GetMovementComponent

// ---------------------------------------------------------------------------------------------------------------------

UPAC_PawnMovementComponent* APAC_BasePawn::GetPawnMovementComponent() const
{
	return MovementComponent;
	
} // GetPawnMovementComponent

// ---------------------------------------------------------------------------------------------------------------------

void APAC_BasePawn::BeginPlay()
{
	Super::BeginPlay();

	SpawnPoint = GetActorTransform();

	AddTickPrerequisiteComponent(MovementComponent);
	
	MovementComponent->StopMovementImmediately();

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	OnRoundReadyHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::ROUND_READY,
		FOnMessageSent::CreateUObject(this, &APAC_BasePawn::OnRoundReady));
	OnSetPausePawnsHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::SET_PAUSE_PAWNS,
		FOnMessageSent::CreateUObject(this, &APAC_BasePawn::OnSetPausePawns));
	OnPlayerDeathHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::PLAYER_DEAD,
		FOnMessageSent::CreateUObject(this, &APAC_BasePawn::OnPlayerDeath));
	OnResetSpawnHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::RESET_SPAWN,
		FOnMessageSent::CreateUObject(this, &APAC_BasePawn::OnResetSpawn));
	OnEndGameHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::END_GAME,
		FOnMessageSent::CreateUObject(this, &APAC_BasePawn::OnEndGame));
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_BasePawn::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	Super::EndPlay(endPlayReason);

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::ROUND_READY, OnRoundReadyHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::SET_PAUSE_PAWNS, OnSetPausePawnsHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::PLAYER_DEAD, OnPlayerDeathHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::RESET_SPAWN, OnResetSpawnHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::END_GAME, OnEndGameHandle);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_BasePawn::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	UpdateAnimations();
	
} // Tick

// ---------------------------------------------------------------------------------------------------------------------

void APAC_BasePawn::OnRoundReady(UPAC_Message* message)
{
	MovementComponent->StartDefaultMovement();
	SetActorTickEnabled(true);
	PaperFlipbook->SetComponentTickEnabled(true);
	MovementComponent->SetComponentTickEnabled(true);
	SetActorHiddenInGame(false);
	
} // OnRoundReady

// ---------------------------------------------------------------------------------------------------------------------

void APAC_BasePawn::OnPlayerDeath(UPAC_Message* message)
{
	SetActorTickEnabled(false);
	PaperFlipbook->SetComponentTickEnabled(false);
	MovementComponent->SetComponentTickEnabled(false);

	GetWorldTimerManager().SetTimer(AfterPlayerDeathTimer, this, &APAC_BasePawn::AfterPlayerDeath, AfterPlayerDeathTime);
	
} // OnPlayerDeath

// ---------------------------------------------------------------------------------------------------------------------

void APAC_BasePawn::OnSetPausePawns(UPAC_Message* message)
{
	const UPAC_BasicMessage* basicMessage = Cast<UPAC_BasicMessage>(message);
	if (basicMessage == nullptr)
	{
		return;
	}
	
	OnSetPausePawnsNative(basicMessage->Bool);
	
} // OnSetPausePawns

// ---------------------------------------------------------------------------------------------------------------------

void APAC_BasePawn::OnSetPausePawnsNative(bool bPause)
{
	SetActorTickEnabled(!bPause);
	PaperFlipbook->SetComponentTickEnabled(!bPause);
	MovementComponent->SetComponentTickEnabled(!bPause);
	
} // OnSetPausePawnsNative

// ---------------------------------------------------------------------------------------------------------------------

void APAC_BasePawn::OnResetSpawn(UPAC_Message* message)
{
	SetActorLocation(SpawnPoint.GetLocation());
	SetActorHiddenInGame(false);
	MovementComponent->SetComponentTickEnabled(false);
	PaperFlipbook->SetComponentTickEnabled(false);
	PaperFlipbook->SetLooping(true);
	PaperFlipbook->SetFlipbook(AnimationByDirection[static_cast<uint8>(OriginalDirection)]);
	SetActorTickEnabled(false);
	
} // OnResetSpawn

// ---------------------------------------------------------------------------------------------------------------------

void APAC_BasePawn::OnEndGame(UPAC_Message* message)
{
	SetActorHiddenInGame(true);
	MovementComponent->SetComponentTickEnabled(false);
	PaperFlipbook->SetComponentTickEnabled(false);
	SetActorTickEnabled(false);
	
} // OnEndGameHandle

// ---------------------------------------------------------------------------------------------------------------------

void APAC_BasePawn::UpdateAnimations()
{
	const FVector& velocity = MovementComponent->Velocity;
	if (FMath::IsNearlyZero(velocity.SizeSquared()))
	{
		if (PaperFlipbook->IsPlaying())
		{
			// Force first frame
			PaperFlipbook->SetNewTime(0.f);
			PaperFlipbook->Stop();
		}
		return;
	}

	// Assume the board is with the base (+X, +Z)
	EPAC_Direction direction = EPAC_Direction::Right;

	const float dotXAxis = velocity | FVector::ForwardVector;
	if (FMath::IsNearlyZero(dotXAxis))
	{
		const float dotYAxis = (velocity | FVector::UpVector) > 0.f;
		direction = dotYAxis > 0.f ? EPAC_Direction::Up : EPAC_Direction::Down;
	}
	else
	{
		direction = dotXAxis > 0.f ? EPAC_Direction::Right : EPAC_Direction::Left;
	}
	
	UPaperFlipbook* animation = AnimationByDirection[static_cast<uint8>(direction)];
	if (PaperFlipbook->GetFlipbook() != animation)
	{
		PaperFlipbook->SetFlipbook(animation);
	}
	
	if (!PaperFlipbook->IsPlaying())
	{
		PaperFlipbook->Play();
	}
	
} // UpdateAnimations

// ---------------------------------------------------------------------------------------------------------------------
