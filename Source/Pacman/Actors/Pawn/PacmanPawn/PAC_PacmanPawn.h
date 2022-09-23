#pragma once

#include "Pacman/Actors/Pawn/PAC_BasePawn.h"
#include "PAC_PacmanPawn.generated.h"

class UPaperSpriteComponent;

UCLASS(Abstract)
class PACMAN_API APAC_PacmanPawn : public APAC_BasePawn
{
	GENERATED_BODY()
	
public:
	APAC_PacmanPawn();

	virtual void PossessedBy(AController* newController) override;
	virtual void OnRoundReady(UPAC_Message* message) override;

	APlayerController* GetPlayerController() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interactable)
	UPaperSpriteComponent* InteractableCollision = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pacman)
	UPaperFlipbook* DeathAnimation = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pacman)
	USoundBase* DeathSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pacman, Meta = (ClampMin = "0", Units = s))
	float NotifyGameStateTime = .5f;

	FTimerHandle NotifyGameStateTimer;

	virtual void AfterPlayerDeath() override;
	virtual void OnSetPausePawnsNative(bool bPause) override;

	UFUNCTION()
	void OnInteractableOverlapped(UPrimitiveComponent* overlappedComponent, AActor* otherActor,
		UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnDeathAnimationEnded();
	
}; // APAC_PacmanPawn
