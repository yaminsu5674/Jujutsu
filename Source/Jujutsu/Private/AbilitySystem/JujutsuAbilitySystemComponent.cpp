// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/JujutsuGameplayAbility.h"
#include "JujutsuGameplayTags.h"
#include "JujutsuTypes/JujutsuEnumTypes.h"

int32 UJujutsuAbilitySystemComponent::HandleGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	// Character.Event.Hit (하위 포함) 수신 시 실행 중인 Character.Ability.HitReact (하위 포함) 어빌리티 취소
	if (EventTag.MatchesTag(JujutsuGameplayTags::Character_Event_Hit))
	{
		FGameplayTagContainer CancelTags;
		CancelTags.AddTag(JujutsuGameplayTags::Character_Ability_HitReact);
		CancelAbilities(&CancelTags, nullptr, nullptr);
	}

	return Super::HandleGameplayEvent(EventTag, Payload);
}

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

bool UJujutsuAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	UE_LOG(LogTemp, Warning, TEXT("TryActivateAbilityByTag: received tag %s"), *AbilityTagToActivate.ToString());

	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty())
	{
		FGameplayAbilitySpec* SpecToActivate = FoundAbilitySpecs[0];
		check(SpecToActivate);

		if (!SpecToActivate->IsActive())
		{
			return TryActivateAbility(SpecToActivate->Handle);
		}
	}

	return false;
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
