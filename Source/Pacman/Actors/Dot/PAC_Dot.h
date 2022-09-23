#pragma once

#include "PaperSpriteActor.h"
#include "Pacman/Interfaces/Eatable/PAC_EatableInterface.h"
#include "PAC_Dot.generated.h"

UCLASS(Abstract)
class PACMAN_API APAC_Dot : public APaperSpriteActor, public IPAC_EatableInterface
{
    GENERATED_BODY()
    
public:
    APAC_Dot();

protected:
    virtual FName GetEatableName() const override;
    virtual UPrimitiveComponent* GetCollision() const override;
    
}; // APAC_Dot
