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

void AJujutsuMultiPlayGameMode::InitializePlayerStarts()
{
	UWorld* World = GetWorld();
	if (!World) return;

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

void AJujutsuMultiPlayGameMode::HandleSeamlessTravelPlayer(AController*& C)
{
	// HandleSeamlessTravelPlayer는 StartPlay보다 먼저 호출될 수 있음. RestartPlayer 시 ChoosePlayerStart가 필요하므로 미리 생성
	if (PlayerStartActors.Num() == 0)
	{
		InitializePlayerStarts();
	}
	// 엔진 기본 로직이 해당 컨트롤러에 대해 RestartPlayer 한 번만 호출. StartPlay 루프 제거로 중복 스폰 방지
	Super::HandleSeamlessTravelPlayer(C);
}

void AJujutsuMultiPlayGameMode::StartPlay()
{
	InitializePlayerStarts();
	Super::StartPlay();
	// RestartPlayer는 HandleSeamlessTravelPlayer에서 플레이어별로 한 번씩만 호출됨. 여기서 루프로 호출하면 중복 스폰(Unpossessed) 발생
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
