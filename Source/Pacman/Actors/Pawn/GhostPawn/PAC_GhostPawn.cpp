#include "PAC_GhostPawn.h"

#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pacman/Actors/Pawn/PAC_PawnMovementComponent.h"
#include "Pacman/Core/MessagingSubsystem/PAC_GameplayEvents.h"
#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "Pacman/Core/MessagingSubsystem/PAC_MessagingSubsystem.h"
#include "Pacman/Settings/PAC_PacmanSettings.h"

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::BeginPlay()
{
	Super::BeginPlay();

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	OnAteThingHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::ATE_THING,
		FOnMessageSent::CreateUObject(this, &APAC_GhostPawn::OnAteThing));
	OnPowerUpGainedHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::POWER_UP_OBTAINED,
		FOnMessageSent::CreateUObject(this, &APAC_GhostPawn::OnPowerUpGained));
	OnPowerUpFinishedHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::POWER_UP_FINISHED,
		FOnMessageSent::CreateUObject(this, &APAC_GhostPawn::OnPowerUpFinished));

	PaperFlipbook->SetCollisionObjectType(bInitSpawn ? ECC_Pawn : GetDefault<UPAC_PacmanSettings>()->PawnSpawnedChannel);
	bInSpawn = bInitSpawn;
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::POWER_UP_OBTAINED, OnPowerUpGainedHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::POWER_UP_FINISHED, OnPowerUpFinishedHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::ATE_THING, OnAteThingHandle);
	
	Super::EndPlay(endPlayReason);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::Enable()
{
	
} // Enable

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::Disable()
{
	
} // Disable

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::OnPlayerAte()
{
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	
	if (!bVulnerable)
	{
		messagingSubsystem->SendEvent(PAC_GameplayEvents::PLAYER_DEAD, nullptr);	
		return;
	}

	GetWorldTimerManager().ClearTimer(VulnerableFinishingTimer);
	messagingSubsystem->SendEvent(PAC_GameplayEvents::SET_PAUSE_PAWNS, UPAC_BasicMessage::CreateBasicMessage(this, true));
	IPAC_EatableInterface::OnPlayerAte();
	
	SetActorHiddenInGame(true);
	GetWorldTimerManager().SetTimer(AteToSpawnTimer, this, &APAC_GhostPawn::BackToSpawn, AteToSpawnTime);
	
} // OnPlayerAte

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::Tick(float deltaSeconds)
{
	Super::Tick(deltaSeconds);

	const bool bHasDelay = GetWorldTimerManager().IsTimerActive(RandomDelayTimer);
	bHasDelay || (bDumb && !bInSpawn) ? RandomWalkWhenHit() : FollowTarget();
	
} // Tick

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::SetVulnerable(bool newVulnerable)
{
	this->bVulnerable = newVulnerable;
	
	constexpr uint8 rightIdx = static_cast<uint8>(EPAC_Direction::Right);
	PaperFlipbook->SetFlipbook(bVulnerable ? VulnerableFlipbook : AnimationByDirection[rightIdx]);

	if (!bVulnerable)
	{
		GetWorldTimerManager().ClearTimer(VulnerableFinishingTimer);
	}
	
} // SetVulnerable

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::OutsideSpawn()
{
	bInSpawn = false;
	PaperFlipbook->SetCollisionObjectType(GetDefault<UPAC_PacmanSettings>()->PawnSpawnedChannel);
	
} // OutsideSpawn

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::RandomWalkWhenHit()
{
	if (!MovementComponent->HasHitLastMove())
	{
		return;
	}

	TArray<FVector> availablePaths = MovementComponent->GetAvailablePaths();
	if (availablePaths.Num() == 0)
	{
		return;
	}
	
	const int32 randomPathIdx = FMath::RandRange(0, availablePaths.Num() - 1);
	MovementComponent->ForceMovementDirection(availablePaths[randomPathIdx]);
	
} // RandomWalk

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::FollowTarget()
{
	if (MovementComponent->HasHitLastMove())
	{
		RandomWalkWhenHit();
		return;
	}

	if (GetWorldTimerManager().IsTimerActive(RecalculatePath))
	{
		return;
	}
	
	TArray<FVector> availablePaths = MovementComponent->GetAvailablePaths();
	if (availablePaths.Num() == 0)
	{
		return;
	}

	const AActor* target = bInSpawn ? OutsideTarget : UGameplayStatics::GetPlayerPawn(this, 0);
	const FVector targetLocation = target->GetActorLocation();
	const FVector diff = targetLocation - GetActorLocation();
	const FVector currentMoveDir = MovementComponent->GetCurrentMovementDirection();
	const float runAway = bVulnerable ? -1.f : 1.f;
	
	TOptional<TPair<float, FVector>> nearestPath;
	for (const FVector& availablePath : availablePaths)
	{
		// Don't go back
		const float dotValCurrentMov = availablePath | currentMoveDir;
		if (FMath::IsNearlyEqual(1.f, -dotValCurrentMov))
		{
			continue;;
		}
		
		const float dotVal = runAway * (diff | availablePath);
		if (!nearestPath.IsSet() || dotVal > nearestPath->Key)
		{
			nearestPath = TPair<float, FVector>{dotVal, availablePath};
		}
	}

	if (!nearestPath.IsSet())
	{
		return;
	}
	
	MovementComponent->ForceMovementDirection(nearestPath->Value);
	static constexpr float WAIT_TIME = .15f;
	GetWorldTimerManager().SetTimer(RecalculatePath, WAIT_TIME, false);
	
} // FollowTarget

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::OnAteThing(UPAC_Message* message)
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

	if (Cast<IPAC_EatableInterface>(eatable)->GetEatableName() == TEXT("Ghost"))
	{
		UpdateMultiplier();
	}
	
} // OnAteThing

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::UpdateMultiplier()
{
	CurrentPointsMultiplier = FMath::Min(16, CurrentPointsMultiplier * 2);
	GetWorldTimerManager().SetTimer(PointsMultiplierTimer, this, &APAC_GhostPawn::ResetMultiplier, PointsMultiplierTime);
	
} // UpdateMultiplier

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::ResetMultiplier()
{
	CurrentPointsMultiplier = 1;
	
} // ResetMultiplier

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::OnPowerUpGained(UPAC_Message* message)
{
	const UPAC_BasicMessage* basicMessage = Cast<UPAC_BasicMessage>(message);
	if (message == nullptr)
	{
		return;
	}

	const float powerUpTime = basicMessage->Float;
	SetVulnerable(true);

	const float finishingTime = .75f * powerUpTime;
	GetWorldTimerManager().SetTimer(VulnerableFinishingTimer, this, &APAC_GhostPawn::SetVulnerableFinishingAnimation,
		finishingTime, false);
	
} // OnPowerUpGained

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::OnPowerUpFinished(UPAC_Message* message)
{
	SetVulnerable(false);
	
} // OnPowerUpFinished

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::SetVulnerableFinishingAnimation()
{
	PaperFlipbook->SetFlipbook(VulnerableFinishingFlipbook);
	
} // SetVulnerableFinishingAnimation

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::UpdateAnimations()
{
	if (!bVulnerable)
	{
		Super::UpdateAnimations();
	}
	
} // UpdateAnimations

// ---------------------------------------------------------------------------------------------------------------------

FName APAC_GhostPawn::GetEatableName() const
{
	return TEXT("Ghost");
	
} // GetEatableName

// ---------------------------------------------------------------------------------------------------------------------

UPrimitiveComponent* APAC_GhostPawn::GetCollision() const
{
	return PaperFlipbook;
	
} // GetCollision

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::AfterPlayerDeath()
{
	Super::AfterPlayerDeath();
	SetActorHiddenInGame(true);
	
} // AfterPlayerDeath

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::OnResetSpawn(UPAC_Message* message)
{
	Super::OnResetSpawn(message);
	
	SetVulnerable(false);
	bInSpawn = bInitSpawn;
	if (bInSpawn)
	{
		PaperFlipbook->SetCollisionObjectType(ECC_Pawn);
	}
	
} // OnResetSpawn

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::BackToSpawn()
{
	SetActorHiddenInGame(false);
	PaperFlipbook->SetFlipbook(DeathFlipbook);
	PaperFlipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static constexpr float TIME_TO_ARRIVE = .5f;
	static constexpr float DELTA_FRAME = 1.f/60.f;
	const FVector diff = SpawnPoint.GetLocation() - GetActorLocation();
	const float distDiff = diff.Size();
	const FVector diffDir = diff / distDiff;
	const FVector offsetPulse = DELTA_FRAME * (distDiff / TIME_TO_ARRIVE) * diffDir;
	
	GetWorldTimerManager().SetTimer(GoToSpawnTimer, FTimerDelegate::CreateWeakLambda(this, [this, offsetPulse]()
	{
		AddActorWorldOffset(offsetPulse);
	}), DELTA_FRAME, true);
	GetWorldTimerManager().SetTimer(ArrivedSpawnTimer, this, &APAC_GhostPawn::ArrivedToSpawn, TIME_TO_ARRIVE);
	
} // BackToSpawn

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::ArrivedToSpawn()
{
	GetWorldTimerManager().ClearTimer(GoToSpawnTimer);
	SetActorLocation(SpawnPoint.GetLocation());

	if (bInitSpawn)
	{
		bInSpawn = true;
		PaperFlipbook->SetCollisionObjectType(ECC_Pawn);
	}

	SetVulnerable(false);
	PaperFlipbook->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->SendEvent(PAC_GameplayEvents::SET_PAUSE_PAWNS, UPAC_BasicMessage::CreateBasicMessage(this, false));

} // ArrivedToSpawn

// ---------------------------------------------------------------------------------------------------------------------

void APAC_GhostPawn::OnRoundReady(UPAC_Message* message)
{
	Super::OnRoundReady(message);

	static constexpr float MIN_DELAY = 0.f;
	static constexpr float MAX_DELAY = 3.f;
	GetWorldTimerManager().SetTimer(RandomDelayTimer, FMath::RandRange(MIN_DELAY, MAX_DELAY), false);
	
} // OnRoundReady

// ---------------------------------------------------------------------------------------------------------------------
