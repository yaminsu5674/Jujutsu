// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/JujutsuSelectionGameMode.h"
#include "JujutsuMultiSelectionGameMode.generated.h"

/** 선택 모드 상속. 멀티 플레이용 선택. TravelServer 호출 시 PlayerState 유지. */
UCLASS()
class JUJUTSU_API AJujutsuMultiSelectionGameMode : public AJujutsuSelectionGameMode
{
	GENERATED_BODY()

public:
	AJujutsuMultiSelectionGameMode();
};
