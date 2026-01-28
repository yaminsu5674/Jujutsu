// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/JujutsuPlayGameMode.h"
#include "JujutsuSinglePlayGameMode.generated.h"

class AJujutsuBaseCharacter;
class UJujutsuGameInstance;

/**
 * 싱글플레이 전용 게임모드
 * - 플레이어 캐릭터: GameInstance 선택 결과 기반 (GetDefaultPawnClassForController)
 * - 적 캐릭터: StartPlay에서 AI로 스폰
 */
UCLASS()
class JUJUTSU_API AJujutsuSinglePlayGameMode : public AJujutsuPlayGameMode
{
	GENERATED_BODY()

protected:
	/** 플레이어가 사용할 Pawn 클래스 결정 (GameInstance HeroCharacterClass 사용) */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	/** 게임 시작 시 AI 적 스폰 */
	virtual void StartPlay() override;

	/** 캐릭터 전원 준비 시 디버그 출력 */
	virtual void OnAllCharactersReady() override;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	FVector EnemySpawnLocation = FVector(300.f, 0.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	FRotator EnemySpawnRotation = FRotator::ZeroRotator;
};
