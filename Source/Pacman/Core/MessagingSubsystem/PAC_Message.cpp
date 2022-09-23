#include "PAC_Message.h"

// ---------------------------------------------------------------------------------------------------------------------

/*static*/ UPAC_BasicMessage* UPAC_BasicMessage::CreateBasicMessage(UObject* owner, int32 intValue)
{
	UPAC_BasicMessage* message = NewObject<UPAC_BasicMessage>(owner);
	message->Int = intValue;
	return message;
	
} // CreateBasicMessage

// ---------------------------------------------------------------------------------------------------------------------

/*static*/ UPAC_BasicMessage* UPAC_BasicMessage::CreateBasicMessage(UObject* owner, bool boolValue)
{
	UPAC_BasicMessage* message = NewObject<UPAC_BasicMessage>(owner);
	message->Bool = boolValue;
	return message;
	
} // CreateBasicMessage

// ---------------------------------------------------------------------------------------------------------------------

/*static*/ UPAC_BasicMessage* UPAC_BasicMessage::CreateBasicMessage(UObject* owner, float floatValue)
{
	UPAC_BasicMessage* message = NewObject<UPAC_BasicMessage>(owner);
	message->Float = floatValue;
	return message;
	
} // CreateBasicMessage

// ---------------------------------------------------------------------------------------------------------------------

/*static*/ UPAC_BasicMessage* UPAC_BasicMessage::CreateBasicMessage(UObject* owner, UObject* objectValue)
{
	UPAC_BasicMessage* message = NewObject<UPAC_BasicMessage>(owner);
	message->Object = objectValue;
	return message;
	
} // CreateBasicMessage

// ---------------------------------------------------------------------------------------------------------------------

UPAC_StringMessage* UPAC_StringMessage::CreateStringMessage(UObject* owner, const FString& stringValue)
{
	UPAC_StringMessage* message = NewObject<UPAC_StringMessage>(owner);
	message->String = stringValue;
	return message;
	
} // CreateStringMessage

// ---------------------------------------------------------------------------------------------------------------------

