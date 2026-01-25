// Copyright Epic Games, Inc. All Rights Reserved.



#include "AbilitySystem/Abilities/JujutsuGameplayAbility.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"

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
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AbilityActivationPolicy == EJujutsuAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UJujutsuCharacterCombatComponent* UJujutsuGameplayAbility::GetCharacterCombatComponentFromActorInfo() const
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	return Avatar ? Avatar->FindComponentByClass<UJujutsuCharacterCombatComponent>() : nullptr;
}
