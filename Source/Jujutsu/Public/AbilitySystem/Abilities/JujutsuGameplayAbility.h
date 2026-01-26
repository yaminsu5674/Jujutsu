// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "JujutsuGameplayAbility.generated.h"

class UGameplayEffect;
class UJujutsuAbilitySystemComponent;
class UJujutsuCharacterCombatComponent;

UENUM(BlueprintType)
enum class EJujutsuAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

/**
 *
 */
UCLASS()
class JUJUTSU_API UJujutsuGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UPROPERTY(EditDefaultsOnly, Category = "JujutsuAbility")
	EJujutsuAbilityActivationPolicy AbilityActivationPolicy = EJujutsuAbilityActivationPolicy::OnTriggered;

	/** 이 어빌리티로 줄 데미지의 기본값 (스킬마다 에디터에서 지정) */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Damage", meta = (ClampMin = "0"))
	float BaseDamage = 10.f;

	UFUNCTION(BlueprintPure, Category = "Jujutsu|Ability")
	UJujutsuCharacterCombatComponent* GetCharacterCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Jujutsu|Ability")
	UJujutsuAbilitySystemComponent* GetJujutsuAbilitySystemComponentFromActorInfo() const;

	/** BaseDamage 멤버를 사용해 데미지용 GE 스펙 생성. InUsedComboCount는 공격 타입 태그의 SetByCaller 값으로 전달 */
	UFUNCTION(BlueprintPure, Category = "Jujutsu|Ability")
	FGameplayEffectSpecHandle MakeDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);
};
