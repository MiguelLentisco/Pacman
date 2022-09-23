#pragma once

#include "PAC_Direction.h"
#include "GameFramework/Pawn.h"
#include "PAC_BasePawn.generated.h"

class UPAC_Message;
class UPAC_PawnMovementComponent;
class UPaperFlipbookComponent;
class UPaperFlipbook;

UCLASS(Abstract)
class PACMAN_API APAC_BasePawn : public APawn
{
	GENERATED_BODY()
	
public:
	APAC_BasePawn();

	virtual UPawnMovementComponent* GetMovementComponent() const override;
	UPAC_PawnMovementComponent* GetPawnMovementComponent() const;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	virtual void Tick(float deltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BasePawn)
	UPaperFlipbookComponent* PaperFlipbook = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BasePawn)
	UPAC_PawnMovementComponent* MovementComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = BasePawn, Meta = (ArraySizeEnum = "EPAC_Direction"))
	UPaperFlipbook* AnimationByDirection[EPAC_Direction::Num];

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BasePawn, Meta = (ClampMin = "0", Units = s))
	float AfterPlayerDeathTime = 1.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BasePawn)
	EPAC_Direction OriginalDirection = EPAC_Direction::Up;

	FTransform SpawnPoint;

	FTimerHandle AfterPlayerDeathTimer;
	
	FDelegateHandle OnRoundReadyHandle;
	FDelegateHandle OnSetPausePawnsHandle;
	FDelegateHandle OnPlayerDeathHandle;
	FDelegateHandle OnResetSpawnHandle;
	FDelegateHandle OnEndGameHandle;

	virtual void OnRoundReady(UPAC_Message* message);
	void OnPlayerDeath(UPAC_Message* message);
	void OnSetPausePawns(UPAC_Message* message);
	virtual void OnSetPausePawnsNative(bool bPause);
	virtual void OnResetSpawn(UPAC_Message* message);
	void OnEndGame(UPAC_Message* message);
	virtual void UpdateAnimations();

	virtual void AfterPlayerDeath() {}
	
}; // APAC_BasePawn
