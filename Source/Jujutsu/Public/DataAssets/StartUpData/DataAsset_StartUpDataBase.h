// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/JujutsuGameplayAbility.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UJujutsuAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FJujutsuCharacterAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UJujutsuGameplayAbility> AbilityToGrant;

	bool IsValid() const { return InputTag.IsValid() && AbilityToGrant; }
};

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

	/** ActivateOnGivenAbilities 뒤에 InAbilities를 append (플레이어컨트롤러 배열 등) */
	void AppendToActivateOnGivenAbilities(const TArray<TSubclassOf<UGameplayAbility>>& InAbilities);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UJujutsuGameplayAbility>> ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UJujutsuGameplayAbility>> ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FJujutsuCharacterAbilitySet> CharacterAbilitySets;

	void GrantAbilities(const TArray<TSubclassOf<UJujutsuGameplayAbility>>& InAbilitiesToGive, UJujutsuAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};
