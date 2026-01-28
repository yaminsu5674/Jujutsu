// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/JujutsuPlayGameMode.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "GameFramework/GameModeBase.h"

void AJujutsuPlayGameMode::NotifyCharacterReady(AJujutsuBaseCharacter* InCharacter)
{
	ReadyCharacterCount++;
	if (ReadyCharacterCount >= MaxCharacterNumber)
	{
		OnAllCharactersReady();
	}
}

void AJujutsuPlayGameMode::OnAllCharactersReady()
{
	// 기본 구현 없음. SinglePlay/MultiPlay에서 오버라이드
}
