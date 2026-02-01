// Copyright Epic Games, Inc. All Rights Reserved.



#include "AbilitySystem/Abilities/JujutsuGameplayAbility.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "JujutsuGameplayTags.h"
#include "JujutsuSkillLibrary.h"

void UJujutsuGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Asset.Tags가 Character_Ability_Attack_* 인 어빌리티는 타겟 방향으로 회전 + 중력 끔
	for (const FGameplayTag& Tag : GetAssetTags())
	{
		if (Tag.MatchesTag(JujutsuGameplayTags::Character_Ability_Attack))
		{
			AJujutsuBaseCharacter* Character = GetCharacterFromActorInfo();
			UJujutsuSkillLibrary::SetActorRotationToTarget(Character);
			UJujutsuSkillLibrary::SetGravityEnabled(Character, false);
			break;
		}
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UJujutsuGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (AbilityActivationPolicy == EJujutsuAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UJujutsuGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Character_Ability_Attack 하위 어빌리티는 종료 시 중력 다시 켬
	for (const FGameplayTag& Tag : GetAssetTags())
	{
		if (Tag.MatchesTag(JujutsuGameplayTags::Character_Ability_Attack))
		{
			UJujutsuSkillLibrary::SetGravityEnabled(GetCharacterFromActorInfo(), true);
			break;
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EJujutsuAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

AJujutsuBaseCharacter* UJujutsuGameplayAbility::GetCharacterFromActorInfo() const
{
	return Cast<AJujutsuBaseCharacter>(GetAvatarActorFromActorInfo());
}

AController* UJujutsuGameplayAbility::GetCharacterControllerFromActorInfo() const
{
	APawn* Avatar = Cast<APawn>(GetAvatarActorFromActorInfo());
	return Avatar ? Avatar->GetController() : nullptr;
}

UJujutsuCharacterCombatComponent* UJujutsuGameplayAbility::GetCharacterCombatComponentFromActorInfo() const
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	return Avatar ? Avatar->FindComponentByClass<UJujutsuCharacterCombatComponent>() : nullptr;
}

UJujutsuAbilitySystemComponent* UJujutsuGameplayAbility::GetJujutsuAbilitySystemComponentFromActorInfo() const
{
	return Cast<UJujutsuAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

FGameplayEffectSpecHandle UJujutsuGameplayAbility::MakeDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, FGameplayTag InCurrentAttackTypeTag, int32 InUsedComboCount)
{
	check(EffectClass);

	UJujutsuAbilitySystemComponent* ASC = GetJujutsuAbilitySystemComponentFromActorInfo();
	if (!ASC) return FGameplayEffectSpecHandle();

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		JujutsuGameplayTags::Character_SetByCaller_BaseDamage,
		BaseDamage
	);

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		JujutsuGameplayTags::Character_SetByCaller_UsedComboCount,
		static_cast<float>(InUsedComboCount)
	);

	return EffectSpecHandle;
}

FActiveGameplayEffectHandle UJujutsuGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	check(TargetASC && InSpecHandle.IsValid());

	return GetJujutsuAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,
		TargetASC
	);
}

FActiveGameplayEffectHandle UJujutsuGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EJujutsuSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveHandle.WasSuccessfullyApplied() ? EJujutsuSuccessType::Successful : EJujutsuSuccessType::Failed;

	return ActiveHandle;
}
