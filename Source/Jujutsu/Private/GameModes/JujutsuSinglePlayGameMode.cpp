// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameModes/JujutsuSinglePlayGameMode.h"
#include "JujutsuGameInstance.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

void AJujutsuSinglePlayGameMode::PostLogin(APlayerController* NewPlayer)
{
	SpawnHeroAndEnemyAndAssignControllers(NewPlayer);
	Super::PostLogin(NewPlayer);
}

void AJujutsuSinglePlayGameMode::PostControllerAssignment()
{
	// 할당 완료 후 C++ 서브클래스에서 오버라이드. 기본은 빈 구현.
}

void AJujutsuSinglePlayGameMode::SpawnHeroAndEnemyAndAssignControllers(APlayerController* PlayerController)
{
	UJujutsuGameInstance* GI = Cast<UJujutsuGameInstance>(GetGameInstance());
	if (!GI || !PlayerController || !GetWorld())
	{
		return;
	}

	TSubclassOf<AJujutsuBaseCharacter> HeroClass = GI->GetHeroCharacterClass();
	TSubclassOf<AJujutsuBaseCharacter> EnemyClass = GI->GetEnemyCharacterClass();
	if (!HeroClass || !EnemyClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 플레이어 캐릭터 스폰 후 플레이어 컨트롤러로 소유
	FTransform HeroTransform(HeroSpawnRotation, HeroSpawnLocation, FVector::OneVector);
	AJujutsuBaseCharacter* HeroPawn = GetWorld()->SpawnActor<AJujutsuBaseCharacter>(HeroClass, HeroTransform, SpawnParams);
	if (HeroPawn)
	{
		PlayerController->Possess(HeroPawn);
	}

	// 적 캐릭터 스폰 후 기본 AI 컨트롤러 스폰·소유 (캐릭터에 설정된 AIControllerClass 사용)
	FTransform EnemyTransform(EnemySpawnRotation, EnemySpawnLocation, FVector::OneVector);
	AJujutsuBaseCharacter* Enemy = GetWorld()->SpawnActor<AJujutsuBaseCharacter>(EnemyClass, EnemyTransform, SpawnParams);
	if (Enemy)
	{
		Enemy->SpawnDefaultController();
	}

	PostControllerAssignment();
}
