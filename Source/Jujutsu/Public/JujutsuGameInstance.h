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

	UFUNCTION(BlueprintCallable, Exec, Category = "Jujutsu|GameInstance")
	void Host(const FString& MapPath);

	UFUNCTION(BlueprintCallable, Exec, Category = "Jujutsu|GameInstance")
	void Join(const FString& Address);

private:
	UPROPERTY()
	TSubclassOf<AJujutsuBaseCharacter> HeroCharacterClass;

	UPROPERTY()
	TSubclassOf<AJujutsuBaseCharacter> EnemyCharacterClass;
};
