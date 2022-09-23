#include "PAC_PointsShower.h"

#include "PaperSpriteComponent.h"
#include "Pacman/Core/MessagingSubsystem/PAC_GameplayEvents.h"
#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "Pacman/Core/MessagingSubsystem/PAC_MessagingSubsystem.h"
#include "Pacman/Interfaces/Eatable/PAC_EatableInterface.h"

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PointsShower::BeginPlay()
{
	Super::BeginPlay();

	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	OnGainedPointsHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::GAINED_POINTS,
		FOnMessageSent::CreateUObject(this, &APAC_PointsShower::OnGainedPoints));
	OnAteThingHandle = messagingSubsystem->AddEventListener(PAC_GameplayEvents::ATE_THING,
		FOnMessageSent::CreateUObject(this, &APAC_PointsShower::OnAteThing));
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PointsShower::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	UPAC_MessagingSubsystem* messagingSubsystem = GetGameInstance()->GetSubsystem<UPAC_MessagingSubsystem>();
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::GAINED_POINTS, OnGainedPointsHandle);
	messagingSubsystem->RemoveEventListener(PAC_GameplayEvents::ATE_THING, OnAteThingHandle);
	
	Super::EndPlay(endPlayReason);
	
} // EndPlay

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PointsShower::OnGainedPoints(UPAC_Message* message)
{
	const UPAC_BasicMessage* basicMessage = Cast<UPAC_BasicMessage>(message);
	if (basicMessage == nullptr)
	{
		return;
	}

	LastPointsGained = basicMessage->Int;
	
} // OnGainedPoints

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PointsShower::OnAteThing(UPAC_Message* message)
{
	const UPAC_BasicMessage* basicMessage = Cast<UPAC_BasicMessage>(message);
	if (basicMessage == nullptr)
	{
		return;
	}

	const AActor* eatable = Cast<AActor>(basicMessage->Object);
	if (eatable == nullptr || !eatable->Implements<UPAC_EatableInterface>())
	{
		return;
	}
	
	const FName name = Cast<IPAC_EatableInterface>(eatable)->GetEatableName();
	if (!SpritePointsByItem.Contains(name))
	{
		return;
	}

	const TMap<int32, UPaperSprite*>& spriteByPoints = SpritePointsByItem[name].SpriteByPoints;
	if (!spriteByPoints.Contains(LastPointsGained))
	{
		return;
	}

	GetRenderComponent()->SetSprite(spriteByPoints[LastPointsGained]);
	SetActorLocation(eatable->GetActorLocation());
	SetActorHiddenInGame(false);
	GetWorldTimerManager().SetTimer(ShowPointsTimer, this, &APAC_PointsShower::HideActor, ShowTime);
	
} // OnAteThing

// ---------------------------------------------------------------------------------------------------------------------

void APAC_PointsShower::HideActor()
{
	SetActorHiddenInGame(true);
	
} // HideActor

// ---------------------------------------------------------------------------------------------------------------------
