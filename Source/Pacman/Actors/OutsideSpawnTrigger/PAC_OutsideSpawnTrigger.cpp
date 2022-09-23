#include "PAC_OutsideSpawnTrigger.h"

#include "Components/ShapeComponent.h"
#include "Pacman/Actors/Pawn/GhostPawn/PAC_GhostPawn.h"

// ---------------------------------------------------------------------------------------------------------------------

// Sets default values
APAC_OutsideSpawnTrigger::APAC_OutsideSpawnTrigger() : Super()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &APAC_OutsideSpawnTrigger::OnGhostEntered);
	GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCollisionComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
} // APAC_OutsideSpawnTrigger

// ---------------------------------------------------------------------------------------------------------------------

void APAC_OutsideSpawnTrigger::OnGhostEntered(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	APAC_GhostPawn* ghostPawn = Cast<APAC_GhostPawn>(otherActor);
	if (ghostPawn == nullptr)
	{
		return;
	}

	ghostPawn->OutsideSpawn();
	
} // OnGhostEntered

// ---------------------------------------------------------------------------------------------------------------------
