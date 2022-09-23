#include "PAC_PawnMovementComponent.h"

// ---------------------------------------------------------------------------------------------------------------------

UPAC_PawnMovementComponent::UPAC_PawnMovementComponent() : Super()
{
	
} // APAC_BasePawn

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);
	SetPlaneConstraintEnabled(true);
	
} // BeginPlay

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PawnMovementComponent::TickComponent(float deltaTime, ELevelTick tickType,
	FActorComponentTickFunction* tickFunc)
{
	Super::TickComponent(deltaTime, tickType, tickFunc);

	bHitLastMove = false;

	// Update last requested direction
	const FVector inputDirection = ConsumeInputVector().GetSafeNormal();
	if (!inputDirection.IsNearlyZero())
	{
		LastRequestedDirection = inputDirection;
	}

	// Check if the request direction has a free path
	if (!FMath::IsNearlyEqual(LastRequestedDirection | CurrentMovementDirection, 1.f) &&
		HasFreePath(LastRequestedDirection))
	{
		CurrentMovementDirection = LastRequestedDirection;
	}
	
	// Update position and velocity
	const float offsetValue = deltaTime * Speed;
	const FVector currentLocation = UpdatedComponent->GetComponentLocation();
	const FQuat rotation = UpdatedComponent->GetComponentQuat();
	const FVector offset = offsetValue * CurrentMovementDirection;

	FHitResult hitResult;
	SafeMoveUpdatedComponent(offset, rotation, true, hitResult);
	if (hitResult.bBlockingHit)
	{
		bHitLastMove = hitResult.bBlockingHit;
		SlideAlongSurface(offset, 1.f - hitResult.Time, hitResult.Normal, hitResult, false);
		if (hitResult.bBlockingHit)
		{
			HandleImpact(hitResult, hitResult.Time, offset);
		}
	}
	
	const FVector newLocation = UpdatedComponent->GetComponentLocation();
	Velocity = (newLocation - currentLocation) / deltaTime;
	
} // TickComponent

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PawnMovementComponent::HandleImpact(const FHitResult& hit, float timeSlice, const FVector& moveDelta)
{
	Super::HandleImpact(hit, timeSlice, moveDelta);
	
	return;

	if (FMath::IsNearlyZero(timeSlice))
	{
		return;
	}

	// Check if there's a path raycasting from center
	static const FVector OFFSET_Y = 2.f * FVector::RightVector;
	static constexpr float RAYCAST_SIZE = 12.f;

	const FVector currentLocation = UpdatedComponent->GetComponentLocation();

	const FVector moveDir = moveDelta.GetSafeNormal();
	const FVector start = currentLocation - OFFSET_Y;
	const FVector end = start + RAYCAST_SIZE * moveDir;
	const ECollisionChannel channel = UpdatedComponent->GetCollisionObjectType();
	FCollisionQueryParams queryParams = FCollisionQueryParams();
	queryParams.bIgnoreTouches = true;
	queryParams.AddIgnoredActor(GetOwner());
	
	FHitResult hitResult;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, channel, queryParams))
	{
		return;
	}

	// Move a small offset perpendicular to the original movement and opposite to the hit
	static constexpr float ROT_ANGLES = 90.f;
	static const FVector ROT_AXIS = FVector::RightVector;
	const FVector fixedMoveDir = moveDir.RotateAngleAxis(ROT_ANGLES, ROT_AXIS);
	
	const FVector fixedOffset = -.75f * (hit.ImpactPoint - currentLocation).ProjectOnToNormal(fixedMoveDir).GetSafeNormal();
	const FQuat rot = UpdatedComponent->GetComponentQuat();
	SafeMoveUpdatedComponent(fixedOffset, rot, true, hitResult);
	
} // HandleImpact

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PawnMovementComponent::StopMovementImmediately()
{
	Super::StopMovementImmediately();
	
	LastRequestedDirection = FVector::ZeroVector;
	CurrentMovementDirection = FVector::ZeroVector;
	
} // StopMovementImmediately

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PawnMovementComponent::StartDefaultMovement()
{
	LastRequestedDirection = FVector::ForwardVector;
	CurrentMovementDirection = FVector::ForwardVector;
	
} // StartDefaultMovement

// ---------------------------------------------------------------------------------------------------------------------

bool UPAC_PawnMovementComponent::HasFreePath(const FVector direction) const
{
	static constexpr float TILE_SIZE = 8.f;

	const FVector currentLocation = UpdatedComponent->GetComponentLocation();	
	const ECollisionChannel channel = UpdatedComponent->GetCollisionObjectType();
	FCollisionQueryParams queryParams = FCollisionQueryParams();
	queryParams.bIgnoreTouches = true;
	queryParams.AddIgnoredActor(GetOwner());

	// Check with raycasts if we can move towards the input direction
	static constexpr float OFFSET_SIZE = TILE_SIZE - 1.5f;
	static constexpr float ROT_ANGLES = 90.f;
	static const FVector ROT_AXIS = FVector::RightVector;
	
	const FVector leftOffset = OFFSET_SIZE * direction.RotateAngleAxis(ROT_ANGLES, ROT_AXIS);
	const FVector rightOffset = OFFSET_SIZE * direction.RotateAngleAxis(-ROT_ANGLES, ROT_AXIS);
	TArray<FVector> raycastOffsets = {FVector::ZeroVector, leftOffset, rightOffset};

	FHitResult hitResult;
	bool bHitFound = false;
	for (const FVector& raycastOffset : raycastOffsets)
	{
		// With no hits, change direction to input direction
		static const FVector OFFSET_Y = 2.f * FVector::RightVector;
		const FVector start = currentLocation + raycastOffset -OFFSET_Y;
		const FVector end = start + 1.5f * TILE_SIZE * direction;
		bHitFound = GetWorld()->LineTraceSingleByChannel(hitResult, start, end, channel, queryParams);
		
		if (bHitFound)
		{
			return false;
		}
	}

	return true;
	
} // HasFreePath

// ---------------------------------------------------------------------------------------------------------------------

TArray<FVector> UPAC_PawnMovementComponent::GetAvailablePaths() const
{
	TArray<FVector> directions = {FVector::ForwardVector, -FVector::ForwardVector, FVector::UpVector, -FVector::UpVector};
	TArray<FVector> availableDirections;
	
	for (const FVector& direction : directions)
	{
		if (HasFreePath(direction))
		{
			availableDirections.Add(direction);
		}
	}

	return availableDirections;
	
} // GetAvailablePaths

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_PawnMovementComponent::ForceMovementDirection(const FVector& direction)
{
	LastRequestedDirection = direction;
	CurrentMovementDirection = direction;
	
} // ForceMovementDirection

// ---------------------------------------------------------------------------------------------------------------------
