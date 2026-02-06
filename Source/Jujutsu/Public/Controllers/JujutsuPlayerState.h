// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "JujutsuPlayerState.generated.h"

/**
 * 플레이어가 고른 캐릭터/적 클래스 타입 저장
 */
UCLASS()
class JUJUTSU_API AJujutsuPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|PlayerState")
	TSubclassOf<AJujutsuBaseCharacter> GetHeroCharacterClass() const { return HeroCharacterClass; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|PlayerState")
	void SetHeroCharacterClass(TSubclassOf<AJujutsuBaseCharacter> InClass) { HeroCharacterClass = InClass; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|PlayerState")
	TSubclassOf<AJujutsuBaseCharacter> GetEnemyCharacterClass() const { return EnemyCharacterClass; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|PlayerState")
	void SetEnemyCharacterClass(TSubclassOf<AJujutsuBaseCharacter> InClass) { EnemyCharacterClass = InClass; }

	/** Seamless Travel 시 새 PlayerState로 Hero/EnemyCharacterClass 복사 */
	virtual void CopyProperties(APlayerState* NewPlayerState) override;

private:
	UPROPERTY()
	TSubclassOf<AJujutsuBaseCharacter> HeroCharacterClass;

	UPROPERTY()
	TSubclassOf<AJujutsuBaseCharacter> EnemyCharacterClass;
};
