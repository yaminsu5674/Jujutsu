// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/JujutsuSelectionGameMode.h"
#include "JujutsuMultiSelectionGameMode.generated.h"

/**
 * 멀티플레이 로비(캐릭터 선택) 전용 GameMode
 * - 모든 플레이어 Ready 시 서버가 판단하여 SeamlessTravel로 전투 맵 이동
 * - bUseSeamlessTravel = true
 */
UCLASS()
class JUJUTSU_API AJujutsuMultiSelectionGameMode : public AJujutsuSelectionGameMode
{
	GENERATED_BODY()

public:
	AJujutsuMultiSelectionGameMode();

	/** PlayerController의 Server_SetReady 호출 후 서버에서 호출. 전체 Ready 조건 검사 */
	void NotifyPlayerReadyStateChanged();

protected:
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Travel")
	FString BattleMapPath = TEXT("/Game/Dynamic/Maps/CityMap/CityMapMulti.CityMapMulti");

	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Ready")
	int32 RequiredPlayerCount = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby|Countdown")
	float CountdownSeconds = 3.f;

private:
	bool bCountdownStarted = false;
	float CountdownRemaining = 0.f;
	int32 LastDisplayedSecond = -1;
};
