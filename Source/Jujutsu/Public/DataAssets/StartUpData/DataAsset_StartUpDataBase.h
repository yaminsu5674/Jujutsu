// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UJujutsuGameplayAbility;
class UJujutsuAbilitySystemComponent;

/**
 * 캐릭터 스타트업 데이터 베이스
 * 지급할 어빌리티 설정 및 ASC에 적용
 */
UCLASS()
class JUJUTSU_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UJujutsuAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UJujutsuGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UJujutsuGameplayAbility>> ReactiveAbilities;

	void GrantAbilities(const TArray<TSubclassOf<UJujutsuGameplayAbility>>& InAbilitiesToGive, UJujutsuAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
