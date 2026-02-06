// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/JujutsuBaseGameMode.h"
#include "Controllers/JujutsuPlayerState.h"

AJujutsuBaseGameMode::AJujutsuBaseGameMode()
{
	PlayerStateClass = AJujutsuPlayerState::StaticClass();
}
