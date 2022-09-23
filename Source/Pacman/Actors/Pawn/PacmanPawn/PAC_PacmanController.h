#pragma once

#include "PAC_PacmanController.generated.h"

UCLASS()
class APAC_PacmanController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void SetupInputComponent() override;

protected:
	void OnXAxis(float value);
	void OnYAxis(float value);
	
}; // APAC_PacmanController
