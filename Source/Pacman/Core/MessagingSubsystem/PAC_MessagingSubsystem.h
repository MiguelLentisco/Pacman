#pragma once

#include "Subsystems/GameInstanceSubsystem.h"
#include "PAC_MessagingSubsystem.generated.h"

class UPAC_Message;

DECLARE_DELEGATE_OneParam(FOnMessageSent, UPAC_Message*)

UCLASS()
class PACMAN_API UPAC_MessagingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	FDelegateHandle AddEventListener(const FName& name, FOnMessageSent listener);
	void RemoveEventListener(const FName& name, const FDelegateHandle& listener);
	void SendEvent(const FName& name, UPAC_Message* message);

protected:
	TMap<FName, TMap<FDelegateHandle, FOnMessageSent>> EventsListeners;
	
}; // UPAC_MessagingSubsystem
