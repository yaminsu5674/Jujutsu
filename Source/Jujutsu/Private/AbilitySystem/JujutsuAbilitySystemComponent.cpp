// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/JujutsuGameplayAbility.h"
#include "JujutsuTypes/JujutsuEnumTypes.h"

void UJujutsuAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UJujutsuAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
	{
		return;
	}

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;
		if (!AbilitySpec.IsActive()) continue;

		FGameplayAbilitySpec* MutableSpec = FindAbilitySpecFromHandle(AbilitySpec.Handle);
		if (!MutableSpec) continue;

		// UE5: 스펙에서 어빌리티 클래스(Def) 또는 CDO 얻기
		UGameplayAbility* Ability = MutableSpec->Ability;
		if (!Ability) continue;

		if (const UJujutsuGameplayAbility* JutsuAbility = Cast<UJujutsuGameplayAbility>(Ability))
		{
			if (JutsuAbility->GetAbilityInputType() == EJujutsuAbilityInputType::Keep)
			{
				CancelAbilityHandle(AbilitySpec.Handle);
			}
		}
	}
}
