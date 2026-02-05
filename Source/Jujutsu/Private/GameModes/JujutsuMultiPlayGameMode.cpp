// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/JujutsuMultiPlayGameMode.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "Controllers/JujutsuPlayerState.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"

UClass* AJujutsuMultiPlayGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (AJujutsuPlayerState* PS = InController ? InController->GetPlayerState<AJujutsuPlayerState>() : nullptr)
	{
		if (TSubclassOf<AJujutsuBaseCharacter> HeroClass = PS->GetHeroCharacterClass())
		{
			return HeroClass.Get();
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

AActor* AJujutsuMultiPlayGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (PlayerStartActors.Num() >= 2)
	{
		int32 PlayersWithPawn = 0;
		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			if (APlayerController* PC = It->Get(); PC && PC != Player && PC->GetPawn())
			{
				PlayersWithPawn++;
			}
		}
		return PlayerStartActors[FMath::Min(PlayersWithPawn, 1)];
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AJujutsuMultiPlayGameMode::StartPlay()
{
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerStartActors.Empty();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (APlayerStart* PS1 = World->SpawnActor<APlayerStart>(Player1Location, Player1Rotation, SpawnParams))
		{
			PlayerStartActors.Add(PS1);
		}
		if (APlayerStart* PS2 = World->SpawnActor<APlayerStart>(Player2Location, Player2Rotation, SpawnParams))
		{
			PlayerStartActors.Add(PS2);
		}
	}

	Super::StartPlay();
}

void AJujutsuMultiPlayGameMode::OnAllCharactersReady()
{
	UWorld* World = GetWorld();
	if (!World) return;

	TArray<AJujutsuBaseCharacter*> Characters;
	for (TActorIterator<AJujutsuBaseCharacter> It(World); It; ++It)
	{
		Characters.Add(*It);
	}

	if (Characters.Num() >= 2)
	{
		for (int32 i = 0; i < Characters.Num(); ++i)
		{
			UJujutsuCharacterCombatComponent* Combat = Characters[i]->GetCharacterCombatComponent();
			if (!Combat) continue;

			int32 TargetIndex = (i + 1) % Characters.Num();
			Combat->Target = Characters[TargetIndex];
		}
	}
}
