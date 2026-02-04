// Copyright Epic Games, Inc. All Rights Reserved.

#include "JujutsuFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"

UJujutsuAbilitySystemComponent* UJujutsuFunctionLibrary::NativeGetJujutsuASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UJujutsuAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UJujutsuFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UJujutsuAbilitySystemComponent* ASC = NativeGetJujutsuASCFromActor(InActor);

	if (!ASC->HasMatchingGameplayTag(TagToAdd))
	{
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UJujutsuFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UJujutsuAbilitySystemComponent* ASC = NativeGetJujutsuASCFromActor(InActor);

	if (ASC->HasMatchingGameplayTag(TagToRemove))
	{
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UJujutsuFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UJujutsuAbilitySystemComponent* ASC = NativeGetJujutsuASCFromActor(InActor);

	return ASC->HasMatchingGameplayTag(TagToCheck);
}

bool UJujutsuFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	return NativeDoesActorHaveTag(InActor, TagToCheck);
}

UJujutsuCharacterCombatComponent* UJujutsuFunctionLibrary::NativeGetCharacterCombatComponentFromActor(AActor* InActor)
{
	check(InActor);

	if (AJujutsuBaseCharacter* BaseCharacter = Cast<AJujutsuBaseCharacter>(InActor))
	{
		return BaseCharacter->GetCharacterCombatComponent();
	}

	return nullptr;
}

UJujutsuCharacterCombatComponent* UJujutsuFunctionLibrary::BP_GetCharacterCombatComponentFromActor(AActor* InActor, EJujutsuValidType& OutValidType)
{
	UJujutsuCharacterCombatComponent* CombatComponent = NativeGetCharacterCombatComponentFromActor(InActor);

	OutValidType = CombatComponent ? EJujutsuValidType::Valid : EJujutsuValidType::Invalid;

	return CombatComponent;
}
