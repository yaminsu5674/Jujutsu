// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/JujutsuSinglePlayGameMode.h"
#include "JujutsuGameInstance.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

// Debug
#include "JujutsuDebugHelper.h"


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

void AJujutsuSinglePlayGameMode::OnAllCharactersReady()
{
	//Debug::Print(TEXT("All ready for setting"));

	UWorld* World = GetWorld();
	if (!World) return;

	TArray<AJujutsuBaseCharacter*> Characters;
	for (TActorIterator<AJujutsuBaseCharacter> It(World); It; ++It)
	{
		Characters.Add(*It);
		if (Characters.Num() >= 2) break;
	}

	if (Characters.Num() == 2 && Characters[0] && Characters[1])
	{
		UJujutsuCharacterCombatComponent* Combat0 = Characters[0]->GetCharacterCombatComponent();
		UJujutsuCharacterCombatComponent* Combat1 = Characters[1]->GetCharacterCombatComponent();
		if (Combat0 && Combat1)
		{
			Combat0->SetTarget(Characters[1]);
			Combat1->SetTarget(Characters[0]);
		}
	}
}
