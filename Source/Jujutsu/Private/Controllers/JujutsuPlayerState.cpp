// Copyright Epic Games, Inc. All Rights Reserved.

#include "Controllers/JujutsuPlayerState.h"

void AJujutsuPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AJujutsuPlayerState, HeroCharacterClass);
	DOREPLIFETIME(AJujutsuPlayerState, EnemyCharacterClass);
	DOREPLIFETIME(AJujutsuPlayerState, bIsReady);
}

void AJujutsuPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
	Super::CopyProperties(NewPlayerState);
	if (AJujutsuPlayerState* JPS = Cast<AJujutsuPlayerState>(NewPlayerState))
	{
		JPS->SetHeroCharacterClass(GetHeroCharacterClass());
		JPS->SetEnemyCharacterClass(GetEnemyCharacterClass());
		JPS->SetIsReady(GetIsReady());
	}
}
