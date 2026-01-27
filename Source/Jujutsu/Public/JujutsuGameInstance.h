// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "JujutsuGameInstance.generated.h"

class AJujutsuBaseCharacter;

UCLASS()
class JUJUTSU_API UJujutsuGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	AJujutsuBaseCharacter* GetHeroCharacter() const { return HeroCharacter; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	void SetHeroCharacter(AJujutsuBaseCharacter* InCharacter) { HeroCharacter = InCharacter; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	AJujutsuBaseCharacter* GetEnemyCharacter() const { return EnemyCharacter; }

	UFUNCTION(BlueprintCallable, Category = "Jujutsu|GameInstance")
	void SetEnemyCharacter(AJujutsuBaseCharacter* InCharacter) { EnemyCharacter = InCharacter; }

private:
	UPROPERTY()
	TObjectPtr<AJujutsuBaseCharacter> HeroCharacter;

	UPROPERTY()
	TObjectPtr<AJujutsuBaseCharacter> EnemyCharacter;
};
