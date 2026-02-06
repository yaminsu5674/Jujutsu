// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "JujutsuGameInstance.generated.h"

UCLASS()
class JUJUTSU_API UJujutsuGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	TSubclassOf<AJujutsuBaseCharacter> GetHeroCharacterClass() const { return HeroCharacterClass; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	void SetHeroCharacterClass(TSubclassOf<AJujutsuBaseCharacter> InClass) { HeroCharacterClass = InClass; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	TSubclassOf<AJujutsuBaseCharacter> GetEnemyCharacterClass() const { return EnemyCharacterClass; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	void SetEnemyCharacterClass(TSubclassOf<AJujutsuBaseCharacter> InClass) { EnemyCharacterClass = InClass; }

	/** Seamless Travel로 플레이 맵 진입 (PlayerState 유지). 호스트 전용. */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	void StartListenServer(const FString& MapName);

	/** 지정 IP 서버에 조인. GameInstance의 Hero/Enemy 선택이 서버 PlayerState로 전달됨. */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	void JoinAsClient(const FString& IPAddress);

	/** Seamless Travel로 로비(멀티선택맵) 복귀 (PlayerState 유지). */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	void LeaveGame();

private:
	UPROPERTY()
	TSubclassOf<AJujutsuBaseCharacter> HeroCharacterClass;

	UPROPERTY()
	TSubclassOf<AJujutsuBaseCharacter> EnemyCharacterClass;
};
