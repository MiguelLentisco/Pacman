#pragma once

#include "GameFramework/PlayerState.h"
#include "Pacman/Core/MessagingSubsystem/PAC_Message.h"
#include "PAC_PlayerState.generated.h"

UCLASS()
class PACMAN_API APAC_PlayerState : public APlayerState
{
GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDataChanged);

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = Pacman)
	FOnPlayerDataChanged OnPlayerLivesChanged;
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, Category = Pacman)
	FOnPlayerDataChanged OnPlayerPointsChanged;

	UFUNCTION(BlueprintSetter)
	void SetCurrentLives(int32 currentLives);
	UFUNCTION(BlueprintSetter)
	void SetCurrentPoints(int32 currentPoints);

	FORCEINLINE int32 GetCurrentLives() const { return CurrentLives; }
	FORCEINLINE int32 GetCurrentPoints() const { return CurrentPoints; }
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, BlueprintSetter = "SetCurrentLives", Category = Pacman)
	int32 CurrentLives = 0;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, BlueprintSetter = "SetCurrentPoints", Category = Pacman)
	int32 CurrentPoints = 0;

	FDelegateHandle GainedPointsHandle;
	FDelegateHandle LostLifeHandle;

	void OnGainedPoints(UPAC_Message* message);
	void OnLostLife(UPAC_Message* message);
	
}; // APAC_PlayerState
