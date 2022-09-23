#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "PAC_PawnMovementComponent.generated.h"

UCLASS(Meta = (BlueprintSpawnableComponent))
class PACMAN_API UPAC_PawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	UPAC_PawnMovementComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* tickFunc) override;
	virtual void HandleImpact(const FHitResult& hit, float timeSlice, const FVector& moveDelta) override;
	
	virtual void StopMovementImmediately() override;
	virtual void StartDefaultMovement();

	FORCEINLINE bool HasHitLastMove() const { return bHitLastMove; }
	bool HasFreePath(const FVector direction) const;
	TArray<FVector> GetAvailablePaths() const;

	void ForceMovementDirection(const FVector& direction);

	FORCEINLINE const FVector& GetCurrentMovementDirection() const { return CurrentMovementDirection; };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, Meta = (ClampMin = 0))
	float Speed = 100.f;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	FVector CurrentMovementDirection = FVector::ForwardVector;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement)
	FVector LastRequestedDirection = FVector::ForwardVector;

	bool bHitLastMove = false;
	
}; // UPAC_PawnMovementComponent
