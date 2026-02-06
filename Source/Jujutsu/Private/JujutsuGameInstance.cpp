// Copyright Epic Games, Inc. All Rights Reserved.

#include "JujutsuGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void UJujutsuGameInstance::StartListenServer(const FString& MapName)
{
	if (UWorld* World = GEngine ? GEngine->GetCurrentPlayWorld() : nullptr)
	{
		const FString MapPath = MapName.Contains(TEXT("/")) ? MapName : FString::Printf(TEXT("/Game/Dynamic/Maps/CityMap/%s.%s"), *MapName, *MapName);
		World->SeamlessTravel(MapPath, true);
	}
}

void UJujutsuGameInstance::JoinAsClient(const FString& IPAddress)
{
	if (UWorld* World = GEngine ? GEngine->GetCurrentPlayWorld() : nullptr)
	{
		if (APlayerController* PC = World->GetFirstPlayerController())
		{
			PC->ClientTravel(IPAddress + TEXT("?join"), TRAVEL_Absolute);
		}
	}
}

void UJujutsuGameInstance::LeaveGame()
{
	if (UWorld* World = GEngine ? GEngine->GetCurrentPlayWorld() : nullptr)
	{
		const FString LobbyPath = TEXT("/Game/Dynamic/Maps/MultiSelectionMap.MultiSelectionMap");
		if (World->GetNetMode() == NM_Client)
		{
			if (APlayerController* PC = World->GetFirstPlayerController())
			{
				PC->ClientTravel(LobbyPath, TRAVEL_Absolute);
			}
		}
		else
		{
			World->SeamlessTravel(LobbyPath, true);
		}
	}
}
