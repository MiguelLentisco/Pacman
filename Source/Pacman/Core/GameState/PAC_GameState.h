#pragma once

#include "GameFramework/GameState.h"
#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "PAC_GameState.generated.h"

class APAC_PlayerState;

UCLASS()
class PACMAN_API APAC_GameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void RoundReady();

	UFUNCTION(BlueprintImplementableEvent)
	void GameFinished(bool win);

	virtual void HandleBeginPlay() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

protected:
	int32 NumDots = 0;
	int32 NumPowerUps = 0;

	UPROPERTY()
	APAC_PlayerState* PlayerState = nullptr;

	FDelegateHandle OnAteThingHandle;

	void OnAteThing(UPAC_Message* message);
	void CheckFinished();

	void EndGame(bool bWin);

	UFUNCTION()
	void CheckLives();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowResetAnimation();
	
}; // APAC_GameState
