// Copyright Epic Games, Inc. All Rights Reserved.

#include "JujutsuFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"

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

void UJujutsuFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EJujutsuConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EJujutsuConfirmType::Yes : EJujutsuConfirmType::No;
}
