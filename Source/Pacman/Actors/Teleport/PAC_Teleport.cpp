#include "PAC_Teleport.h"

#include "Components/ShapeComponent.h"

// ---------------------------------------------------------------------------------------------------------------------

APAC_Teleport::APAC_Teleport() : Super()
{
	PrimaryActorTick.bCanEverTick = false;

	UShapeComponent* collision = GetCollisionComponent();
	collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	collision->OnComponentBeginOverlap.AddDynamic(this, &APAC_Teleport::OnPawnBeginOverlap);
	
} // APAC_Teleport

// ---------------------------------------------------------------------------------------------------------------------

void APAC_Teleport::OnPawnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	APawn* pawn = Cast<APawn>(otherActor);
	if (pawn == nullptr || TeleportPoint == nullptr)
	{
		return;
	}

	pawn->TeleportTo(TeleportPoint->GetActorLocation(), pawn->GetActorRotation());
	
} // OnPawnBeginOverlap

// ---------------------------------------------------------------------------------------------------------------------
