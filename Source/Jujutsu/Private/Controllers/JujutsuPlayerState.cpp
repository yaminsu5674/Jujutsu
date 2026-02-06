// Copyright Epic Games, Inc. All Rights Reserved.

#include "Controllers/JujutsuPlayerState.h"

void AJujutsuPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
	Super::CopyProperties(NewPlayerState);
	if (AJujutsuPlayerState* JPS = Cast<AJujutsuPlayerState>(NewPlayerState))
	{
		JPS->SetHeroCharacterClass(GetHeroCharacterClass());
		JPS->SetEnemyCharacterClass(GetEnemyCharacterClass());
	}
}
