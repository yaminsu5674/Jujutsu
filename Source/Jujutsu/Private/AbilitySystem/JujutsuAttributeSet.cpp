// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/JujutsuAttributeSet.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/UI/CharacterUIComponent.h"
#include "GameplayEffectExtension.h"
#include "JujutsuFunctionLibrary.h"
#include "JujutsuGameplayTags.h"
#include "Net/UnrealNetwork.h"

// Debug
#include "JujutsuDebugHelper.h"

UJujutsuAttributeSet::UJujutsuAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UJujutsuAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UJujutsuAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UJujutsuAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UJujutsuAttributeSet, CurrentRage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UJujutsuAttributeSet, MaxRage, COND_None, REPNOTIFY_Always);
}

void UJujutsuAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldCurrentHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UJujutsuAttributeSet, CurrentHealth, OldCurrentHealth);
}

void UJujutsuAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UJujutsuAttributeSet, MaxHealth, OldMaxHealth);
}

void UJujutsuAttributeSet::OnRep_CurrentRage(const FGameplayAttributeData& OldCurrentRage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UJujutsuAttributeSet, CurrentRage, OldCurrentRage);
}

void UJujutsuAttributeSet::OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UJujutsuAttributeSet, MaxRage, OldMaxRage);
}

void UJujutsuAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);
	}

	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		if (GetCurrentHealth() == 0.f)
		{
			AActor* AvatarActor = Data.Target.GetAvatarActor();
			UJujutsuFunctionLibrary::AddGameplayTagToActorIfNone(AvatarActor, JujutsuGameplayTags::Character_Status_Dead);
			UJujutsuFunctionLibrary::SendGameplayEventToActorAuthorityOnly(AvatarActor, JujutsuGameplayTags::Character_Event_Death, FGameplayEventData(), AvatarActor);
		}
	}
}
