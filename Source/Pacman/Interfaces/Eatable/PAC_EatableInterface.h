#pragma once

#include "PAC_EatableInterface.generated.h"

struct FPAC_EatableData;

UINTERFACE(MinimalAPI)
class UPAC_EatableInterface : public UInterface
{
	GENERATED_BODY()
	
}; // UPAC_EatableInterface

class PACMAN_API IPAC_EatableInterface
{
	GENERATED_BODY()
	
public:
	virtual void Enable();
	virtual void Disable();
	virtual void OnPlayerAte();

	virtual FName GetEatableName() const { return TEXT(""); }

protected:
	virtual UPrimitiveComponent* GetCollision() const { return nullptr; }
	virtual int32 GetPointsMultiplier() const { return 1; }

	FPAC_EatableData* GetEatableData() const;
	
}; // IPAC_EatableInterface