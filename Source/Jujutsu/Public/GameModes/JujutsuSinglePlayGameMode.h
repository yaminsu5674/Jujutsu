// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/JujutsuPlayGameMode.h"
#include "JujutsuSinglePlayGameMode.generated.h"

class UJujutsuGameInstance;
class AJujutsuBaseCharacter;
class APlayerController;

/** 플레이 모드 상속. 싱글 플레이용 대전. */
UCLASS()
class JUJUTSU_API AJujutsuSinglePlayGameMode : public AJujutsuPlayGameMode
{
	GENERATED_BODY()

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	/** 플레이어·적 스폰 후 컨트롤러 할당까지 끝난 뒤 호출. C++ 서브클래스에서 오버라이드. */
	virtual void PostControllerAssignment();

private:
	void SpawnHeroAndEnemyAndAssignControllers(APlayerController* PlayerController);

	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|SinglePlay|Spawn")
	FVector HeroSpawnLocation = FVector(0.f, 0.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|SinglePlay|Spawn")
	FRotator HeroSpawnRotation = FRotator::ZeroRotator;

	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|SinglePlay|Spawn")
	FVector EnemySpawnLocation = FVector(400.f, 0.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|SinglePlay|Spawn")
	FRotator EnemySpawnRotation = FRotator::ZeroRotator;
};
