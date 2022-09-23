#pragma once

#include "Pacman/Actors/Pawn/PAC_BasePawn.h"
#include "Pacman/Interfaces/Eatable/PAC_EatableInterface.h"
#include "PAC_GhostPawn.generated.h"

UCLASS(Abstract)
class PACMAN_API APAC_GhostPawn : public APAC_BasePawn, public IPAC_EatableInterface
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
	
	virtual void Enable() override;
	virtual void Disable() override;
	virtual void OnPlayerAte() override;
	virtual int32 GetPointsMultiplier() const override { return CurrentPointsMultiplier; }

	virtual void Tick(float deltaSeconds) override;

	void SetVulnerable(bool newVulnerable);
	void OutsideSpawn();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ghost)
	bool bDumb = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ghost)
	bool bInitSpawn = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ghost)
	AActor* OutsideTarget = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ghost, Meta = (ClampMin = 0, Units = s))
	float PointsMultiplierTime = 5.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ghost, Meta = (ClampMin = 0, Units = s))
	float AteToSpawnTime = 1.25f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ghost)
	UPaperFlipbook* VulnerableFlipbook = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ghost)
	UPaperFlipbook* VulnerableFinishingFlipbook = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ghost)
	UPaperFlipbook* DeathFlipbook = nullptr;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Ghost)
	bool bVulnerable = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Ghost)
	bool bInSpawn = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Ghost)
	int32 CurrentPointsMultiplier = 1;

	FDelegateHandle OnAteThingHandle;
	FDelegateHandle OnPowerUpGainedHandle;
	FDelegateHandle OnPowerUpFinishedHandle;

	FTimerHandle VulnerableFinishingTimer;
	FTimerHandle PointsMultiplierTimer;
	FTimerHandle AteToSpawnTimer;
	FTimerHandle GoToSpawnTimer;
	FTimerHandle ArrivedSpawnTimer;
	FTimerHandle RecalculatePath;
	FTimerHandle RandomDelayTimer;

	void RandomWalkWhenHit();
	void FollowTarget();

	void OnAteThing(UPAC_Message* message);
	void UpdateMultiplier();
	void ResetMultiplier();
	
	void OnPowerUpGained(UPAC_Message* message);
	void OnPowerUpFinished(UPAC_Message* message);
	void SetVulnerableFinishingAnimation();

	virtual void UpdateAnimations() override;
	
	virtual FName GetEatableName() const override;
	virtual UPrimitiveComponent* GetCollision() const override;

	virtual void AfterPlayerDeath() override;
	virtual void OnResetSpawn(UPAC_Message* message) override;

	void BackToSpawn();
	void ArrivedToSpawn();

	virtual void OnRoundReady(UPAC_Message* message) override;
	
}; // APAC_GhostPawn
