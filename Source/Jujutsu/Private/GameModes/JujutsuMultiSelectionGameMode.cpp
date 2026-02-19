// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/JujutsuMultiSelectionGameMode.h"
#include "Controllers/JujutsuPlayerState.h"
#include "Controllers/JujutsuHeroController.h"
#include "JujutsuGameInstance.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogMultiSelectionLobby, Log, All);

AJujutsuMultiSelectionGameMode::AJujutsuMultiSelectionGameMode()
{
	bUseSeamlessTravel = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AJujutsuMultiSelectionGameMode::NotifyPlayerReadyStateChanged()
{
	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() == NM_Client)
	{
		UE_LOG(LogMultiSelectionLobby, Verbose, TEXT("[NotifyPlayerReadyStateChanged] SKIP - Client or no World"));
		return;
	}
	if (bCountdownStarted) return;

	int32 ReadyCount = 0;
	int32 TotalCount = 0;
	FString PlayerStatus;
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		if (APlayerController* PC = It->Get())
		{
			TotalCount++;
			if (AJujutsuPlayerState* PS = PC->GetPlayerState<AJujutsuPlayerState>())
			{
				const bool bReady = PS->GetIsReady();
				const bool bHasChar = PS->GetHeroCharacterClass() != nullptr;
				if (bReady && bHasChar) ReadyCount++;
				PlayerStatus += FString::Printf(TEXT("[P%d:Ready=%d,Char=%d] "), TotalCount - 1, bReady, bHasChar);
			}
		}
	}

	UE_LOG(LogMultiSelectionLobby, Log, TEXT("[NotifyPlayerReadyStateChanged] NetMode=%d Total=%d Ready=%d Required=%d %s"),
		(int32)World->GetNetMode(), TotalCount, ReadyCount, RequiredPlayerCount, *PlayerStatus);

	if (TotalCount >= RequiredPlayerCount && ReadyCount >= RequiredPlayerCount)
	{
		bCountdownStarted = true;
		CountdownRemaining = CountdownSeconds;
		UE_LOG(LogMultiSelectionLobby, Warning, TEXT("[Countdown START] Server will SeamlessTravel in %.1f sec to %s"), CountdownSeconds, *BattleMapPath);
	}
}

void AJujutsuMultiSelectionGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UWorld* World = GetWorld();
	if (!World || World->GetNetMode() == NM_Client)
	{
		return;
	}
	if (!bCountdownStarted || CountdownRemaining <= 0.f) return;

	CountdownRemaining -= DeltaSeconds;
	const int32 SecondsRemaining = FMath::Max(0, FMath::CeilToInt(CountdownRemaining));

	if (SecondsRemaining > 0)
	{
		if (SecondsRemaining != LastDisplayedSecond)
		{
			LastDisplayedSecond = SecondsRemaining;
			int32 ClientCount = 0;
			for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
			{
				if (AJujutsuHeroController* HC = Cast<AJujutsuHeroController>(It->Get()))
				{
					HC->Client_ShowCountdown(SecondsRemaining);
					ClientCount++;
				}
			}
			UE_LOG(LogMultiSelectionLobby, Log, TEXT("[Countdown] %d sec left, Client_ShowCountdown sent to %d controllers"), SecondsRemaining, ClientCount);
		}
	}
	else
	{
		if (World->GetNetMode() != NM_ListenServer && World->GetNetMode() != NM_DedicatedServer)
		{
			UE_LOG(LogMultiSelectionLobby, Error, TEXT("[SeamlessTravel] ABORT - Not server! NetMode=%d"), (int32)World->GetNetMode());
			return;
		}
		UE_LOG(LogMultiSelectionLobby, Warning, TEXT("[Travel] Server calling GameInstance::Host. URL=%s"), *BattleMapPath);
		if (UJujutsuGameInstance* GI = World->GetGameInstance<UJujutsuGameInstance>())
		{
			FString TravelURL = FString::Printf(TEXT("%s?game=%s"), *BattleMapPath, *MultiGameModePath);
			GI->Host(TravelURL, true);
		}
		UE_LOG(LogMultiSelectionLobby, Warning, TEXT("[Travel] Host call returned"));
	}
}
