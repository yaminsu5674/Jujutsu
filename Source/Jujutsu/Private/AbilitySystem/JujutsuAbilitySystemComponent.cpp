// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/JujutsuAbilitySystemComponent.h"

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
}
