// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/JujutsuSinglePlayGameMode.h"
#include "JujutsuGameInstance.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"


UClass* AJujutsuSinglePlayGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (UJujutsuGameInstance* GI = GetGameInstance<UJujutsuGameInstance>())
	{
		if (TSubclassOf<AJujutsuBaseCharacter> HeroClass = GI->GetHeroCharacterClass())
		{
			return HeroClass.Get();
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void AJujutsuSinglePlayGameMode::StartPlay()
{
	Super::StartPlay();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UJujutsuGameInstance* GI = GetGameInstance<UJujutsuGameInstance>();
	if (!GI || !GI->GetEnemyCharacterClass())
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AJujutsuBaseCharacter* Enemy = World->SpawnActor<AJujutsuBaseCharacter>(
		GI->GetEnemyCharacterClass(),
		EnemySpawnLocation,
		EnemySpawnRotation,
		SpawnParams
	);

	if (Enemy)
	{
		Enemy->SpawnDefaultController();
	}
}
