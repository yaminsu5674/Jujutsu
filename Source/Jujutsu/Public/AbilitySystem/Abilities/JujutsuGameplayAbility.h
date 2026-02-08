// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "JujutsuTypes/JujutsuEnumTypes.h"
#include "JujutsuGameplayAbility.generated.h"

class AJujutsuBaseCharacter;
class UGameplayEffect;
class UJujutsuAbilitySystemComponent;
class UJujutsuCharacterCombatComponent;

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
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~ End UGameplayAbility Interface

	UPROPERTY(EditDefaultsOnly, Category = "JujutsuAbility")
	EJujutsuAbilityActivationPolicy AbilityActivationPolicy = EJujutsuAbilityActivationPolicy::OnTriggered;

	/** 키 타입: Pressed=누르면 한 번 실행, Keep=누르고 있는 동안 활성·키 떼면 EndAbility */
	UPROPERTY(EditDefaultsOnly, Category = "JujutsuAbility")
	EJujutsuAbilityInputType AbilityInputType = EJujutsuAbilityInputType::Pressed;

public:
	FORCEINLINE EJujutsuAbilityInputType GetAbilityInputType() const { return AbilityInputType; }

	/** AI용: 이 어빌리티 사용 시 원하는 거리 (0 이하일 경우 사용 안 함 등) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jujutsu|AI", meta = (ClampMin = "0"))
	float DesiredRange = 0.f;

protected:
	/** 이 어빌리티로 줄 데미지의 기본값 (스킬마다 에디터에서 지정) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jujutsu|Damage", meta = (ClampMin = "0"))
	float BaseDamage = 10.f;

	UFUNCTION(BlueprintPure, Category = "Jujutsu|Ability")
	AJujutsuBaseCharacter* GetCharacterFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Jujutsu|Ability")
	AController* GetCharacterControllerFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Jujutsu|Ability")
	UJujutsuCharacterCombatComponent* GetCharacterCombatComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure, Category = "Jujutsu|Ability")
	UJujutsuAbilitySystemComponent* GetJujutsuAbilitySystemComponentFromActorInfo() const;

	/** BaseDamage 멤버를 사용해 데미지용 GE 스펙 생성. InUsedComboCount는 Character_SetByCaller_UsedComboCount로 SetByCaller 전달 */
	UFUNCTION(BlueprintPure, Category = "Jujutsu|Ability")
	FGameplayEffectSpecHandle MakeDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount);

	/** 타겟 액터의 ASC에 EffectSpec 적용 (네이티브용) */
	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);

	/** 타겟 액터의 ASC에 EffectSpec 적용. 블루프린트용, OutSuccessType으로 성공 여부 분기 */
	UFUNCTION(BlueprintCallable, Category = "Jujutsu|Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EJujutsuSuccessType& OutSuccessType);
};
