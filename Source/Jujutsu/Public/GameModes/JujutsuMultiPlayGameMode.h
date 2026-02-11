// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/JujutsuPlayGameMode.h"
#include "JujutsuMultiPlayGameMode.generated.h"

class APlayerStart;

/**
 * 멀티플레이 전용 게임모드 (최대 2인)
 * - 플레이어 캐릭터: 각 PlayerState의 HeroCharacterClass 사용 (GetDefaultPawnClassForController)
 * - 적 스폰 없음. 플레이어 2명만 입장.
 */
UCLASS()
class JUJUTSU_API AJujutsuMultiPlayGameMode : public AJujutsuPlayGameMode
{
	GENERATED_BODY()

protected:
	/** 각 플레이어가 사용할 Pawn 클래스 결정 (해당 컨트롤러의 PlayerState HeroCharacterClass 사용) */
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void StartPlay() override;

	/** Seamless Travel 시 플레이어별로 한 번만 호출됨. 여기서 Super 호출로 RestartPlayer 실행. StartPlay 루프와 중복되지 않도록 루프 제거 */
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;

	/** 캐릭터 전원 준비 시 타겟 설정 */
	virtual void OnAllCharactersReady() override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn|Player1")
	FVector Player1Location = FVector(-200.f, 0.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Spawn|Player1")
	FRotator Player1Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn|Player2")
	FVector Player2Location = FVector(200.f, 0.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Spawn|Player2")
	FRotator Player2Rotation = FRotator::ZeroRotator;

	/** PlayerStart 스폰. HandleSeamlessTravelPlayer(StartPlay보다 먼저 호출될 수 있음)와 StartPlay 양쪽에서 사용 */
	void InitializePlayerStarts();

	TArray<APlayerStart*> PlayerStartActors;
};
