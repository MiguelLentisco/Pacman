#pragma once

#include "UObject/Object.h"
#include "PAC_Message.generated.h"

UCLASS(Abstract)
class PACMAN_API UPAC_Message : public UObject
{
	GENERATED_BODY()
	
}; // UPAC_Message

UCLASS()
class PACMAN_API UPAC_BasicMessage : public UPAC_Message
{
	GENERATED_BODY()
	
public:
	union
	{
		int32 Int;
		bool Bool;
		float Float;
		UObject* Object;
	};
	
	static UPAC_BasicMessage* CreateBasicMessage(UObject* owner, int32 intValue);
	static UPAC_BasicMessage* CreateBasicMessage(UObject* owner, bool boolValue);
	static UPAC_BasicMessage* CreateBasicMessage(UObject* owner, float floatValue);
	static UPAC_BasicMessage* CreateBasicMessage(UObject* owner, UObject* objectValue);
	
}; // UPAC_BasicMessage

UCLASS()
class PACMAN_API UPAC_StringMessage : public UPAC_Message
{
	GENERATED_BODY()
	
public:
	FString String;
	
	static UPAC_StringMessage* CreateStringMessage(UObject* owner, const FString& stringValue);
	
}; // UPAC_StringMessage