#include "PacmanGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "Pacman/Core/PlayerState/PAC_PlayerState.h"

// ---------------------------------------------------------------------------------------------------------------------

void APacmanGameModeBase::StartPlay()
{
	DisableToneMapper();

	const APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APAC_PlayerState* playerState = playerController->GetPlayerState<APAC_PlayerState>();
	playerState->SetCurrentLives(InitialLives);
	
	Super::StartPlay();
	
} // StartPlay

// ---------------------------------------------------------------------------------------------------------------------

void APacmanGameModeBase::DisableToneMapper() const
{
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	// Disable ToneMapping from https://soldirix.wordpress.com/2020/05/26/ue4-how-to-disable-the-tone-mapper-once-and-for-all/
	// The command being executed.
	FString Command = "ShowFlag.Tonemapper 0";
	playerController->ConsoleCommand(*Command);

	// The below commands are for the shipping
	// build.
	Command = "r.TonemapperGamma 0";
	playerController->ConsoleCommand(*Command);

	Command = "r.TonemapperFilm 0";
	playerController->ConsoleCommand(*Command);

	Command = "r.Tonemapper.Sharpen 0";
	playerController->ConsoleCommand(*Command);

	Command = "r.Tonemapper.GrainQuantization 0";
	playerController->ConsoleCommand(*Command);

	Command = "r.Tonemapper.Quality 0";
	playerController->ConsoleCommand(*Command);
	
} // DisableToneMapper

// ---------------------------------------------------------------------------------------------------------------------
