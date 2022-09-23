#include "PAC_MessagingSubsystem.h"

// ---------------------------------------------------------------------------------------------------------------------

FDelegateHandle UPAC_MessagingSubsystem::AddEventListener(const FName& name, FOnMessageSent listener)
{
	if (name.IsNone())
	{
		return FDelegateHandle();
	}
	
	if (!EventsListeners.Contains(name))
	{
		EventsListeners.Add(name, {});
	}

	const FDelegateHandle listenerHandle = listener.GetHandle();
	EventsListeners[name].Add(listenerHandle, listener);
	return listenerHandle;
	
} // AddEvent

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_MessagingSubsystem::RemoveEventListener(const FName& name, const FDelegateHandle& listener)
{
	if (!EventsListeners.Contains(name))
	{
		return;
	}
	
	EventsListeners[name].Remove(listener);
	
} // RemoveEventListener

// ---------------------------------------------------------------------------------------------------------------------

void UPAC_MessagingSubsystem::SendEvent(const FName& name, UPAC_Message* message)
{
	if (!EventsListeners.Contains(name))
	{
		return;
	}

	for (const TPair<FDelegateHandle, FOnMessageSent>& listener : EventsListeners[name])
	{
		listener.Value.ExecuteIfBound(message);
	}
	
} // SendEvent

// ---------------------------------------------------------------------------------------------------------------------

