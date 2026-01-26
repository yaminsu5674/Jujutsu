// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/JujutsuAttributeSet.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/UI/CharacterUIComponent.h"
#include "GameplayEffectExtension.h"
#include "JujutsuDebugHelper.h"
#include "JujutsuFunctionLibrary.h"
#include "JujutsuGameplayTags.h"

UJujutsuAttributeSet::UJujutsuAttributeSet()
{
	InitCurrentHealth(1.f);
	InitMaxHealth(1.f);
	InitCurrentRage(1.f);
	InitMaxRage(1.f);
	InitAttackPower(1.f);
	InitDefensePower(1.f);
}

void UJujutsuAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (!CachedCharacterUIComponent.IsValid())
	{
		if (AActor* Avatar = Data.Target.GetAvatarActor())
		{
			if (AJujutsuBaseCharacter* Char = Cast<AJujutsuBaseCharacter>(Avatar))
			{
				CachedCharacterUIComponent = Char->GetCharacterUIComponent();
			}
		}
	}

	UCharacterUIComponent* UIComp = CachedCharacterUIComponent.Get();

	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetCurrentHealth(), 0.f, GetMaxHealth());
		SetCurrentHealth(NewCurrentHealth);

		if (UIComp)
		{
			UIComp->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
		}
	}

	if (Data.EvaluatedData.Attribute == GetCurrentRageAttribute())
	{
		const float NewCurrentRage = FMath::Clamp(GetCurrentRage(), 0.f, GetMaxRage());
		SetCurrentRage(NewCurrentRage);

		if (UIComp)
		{
			UIComp->OnCurrentRageChanged.Broadcast(GetCurrentRage() / GetMaxRage());
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetCurrentHealth();
		const float DamageDone = GetDamageTaken();
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());

		SetCurrentHealth(NewCurrentHealth);

		const FString DebugString = FString::Printf(
			TEXT("Old Health: %f, Damage Done: %f, NewCurrentHealth: %f"),
			OldHealth,
			DamageDone,
			NewCurrentHealth
		);
		Debug::Print(DebugString, FColor::Green);

		if (UIComp)
		{
			UIComp->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
		}

		if (NewCurrentHealth == 0.f)
		{
			UJujutsuFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), JujutsuGameplayTags::Character_Status_Dead);
		}
	}
}
