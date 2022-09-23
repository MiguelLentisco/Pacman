#include "PAC_EatableInterface.h"

#include "Pacman/Data/EatableData/PAC_EatableData.h"
#include "Pacman/Settings/PAC_PacmanSettings.h"
#include "Pacman/Core/MessagingSubsystem/PAC_GameplayEvents.h"
#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "Pacman/Core/MessagingSubsystem/PAC_MessagingSubsystem.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

// ---------------------------------------------------------------------------------------------------------------------

void IPAC_EatableInterface::Enable()
{
	UPrimitiveComponent* collision = GetCollision();
	if (collision == nullptr)
	{
		return;
	}

	collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collision->SetHiddenInGame(false);
	
} // Enable

// ---------------------------------------------------------------------------------------------------------------------

void IPAC_EatableInterface::Disable()
{
	UPrimitiveComponent* collision = GetCollision();
	if (collision == nullptr)
	{
		return;
	}

	collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	collision->SetHiddenInGame(true);
	
} // Disable

// ---------------------------------------------------------------------------------------------------------------------

void IPAC_EatableInterface::OnPlayerAte()
{
	Disable();

	const UPrimitiveComponent* collision = GetCollision();
	if (collision == nullptr)
	{
		return;
	}

	const FPAC_EatableData* eatableData = GetEatableData();
	if (eatableData == nullptr)
	{
		return;
	}

	UObject* owner = collision->GetOwner();
	const UGameInstance* gameInstance = UGameplayStatics::GetGameInstance(collision);
	UPAC_MessagingSubsystem* messagingSubsystem = gameInstance->GetSubsystem<UPAC_MessagingSubsystem>();

	const int32 finalPoints = GetPointsMultiplier() * eatableData->Points;
	messagingSubsystem->SendEvent(PAC_GameplayEvents::GAINED_POINTS, UPAC_BasicMessage::CreateBasicMessage(owner, finalPoints));
	messagingSubsystem->SendEvent(PAC_GameplayEvents::PLAY_FX_SOUND, UPAC_BasicMessage::CreateBasicMessage(owner, eatableData->Sound.LoadSynchronous()));
	messagingSubsystem->SendEvent(PAC_GameplayEvents::ATE_THING, UPAC_BasicMessage::CreateBasicMessage(owner, collision->GetOwner()));
	
} // OnPlayerAte

// ---------------------------------------------------------------------------------------------------------------------

FPAC_EatableData* IPAC_EatableInterface::GetEatableData() const
{
	const UDataTable* eatableDT = GetDefault<UPAC_PacmanSettings>()->EatableDataTable.LoadSynchronous();
	if (eatableDT == nullptr)
	{
		return nullptr;
	}
	
    static const FString CONTEXT = TEXT("[EatableInterface] GetPoints");
	return eatableDT->FindRow<FPAC_EatableData>(GetEatableName(), CONTEXT);
	
} // GetPoints

// ---------------------------------------------------------------------------------------------------------------------
