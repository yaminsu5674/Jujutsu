// Copyright Epic Games, Inc. All Rights Reserved.

#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/JujutsuGameplayAbility.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "Controllers/JujutsuHeroController.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UJujutsuAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);
	GrantAbilitiesFromPlayerController(InASCToGive, ApplyLevel);

	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			if (!EffectClass) continue;

			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();

			InASCToGive->ApplyGameplayEffectToSelf(
				EffectCDO,
				ApplyLevel,
				InASCToGive->MakeEffectContext()
			);
		}
	}

	for (const FJujutsuCharacterAbilitySet& AbilitySet : CharacterAbilitySets)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		InASCToGive->GiveAbility(AbilitySpec);
	}
}

void UDataAsset_StartUpDataBase::GrantAbilitiesFromPlayerController(UJujutsuAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (!InASCToGive) return;

	AActor* Avatar = InASCToGive->GetAvatarActor();
	AController* Controller = nullptr;
	if (APawn* AvatarPawn = Cast<APawn>(Avatar))
	{
		Controller = AvatarPawn->GetController();
	}

	AJujutsuHeroController* HeroController = Cast<AJujutsuHeroController>(Controller);
	if (!HeroController) return;

	const TArray<TSubclassOf<UGameplayAbility>>& ControllerAbilities = HeroController->GetAbilitiesToGrantWithCharacterInit();
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : ControllerAbilities)
	{
		if (!AbilityClass || !AbilityClass->IsChildOf(UJujutsuGameplayAbility::StaticClass())) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityClass);
		AbilitySpec.SourceObject = Avatar;
		AbilitySpec.Level = ApplyLevel;
		InASCToGive->GiveAbility(AbilitySpec);
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UJujutsuGameplayAbility>>& InAbilitiesToGive, UJujutsuAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())
	{
		return;
	}

	for (const TSubclassOf<UJujutsuGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InASCToGive->GiveAbility(AbilitySpec);
	}
}
