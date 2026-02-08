// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/GameplayEffect/GE_DashCooldown.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "JujutsuGameplayTags.h"

UGE_DashCooldown::UGE_DashCooldown(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
}

void UGE_DashCooldown::PostInitProperties()
{
	Super::PostInitProperties();

	// Granted Tags: Cooldown.Dash (생성자에서 AddComponent 시 NewObject/NAME_None 오류 발생하므로 PostInitProperties에서 수행)
	UTargetTagsGameplayEffectComponent& TargetTagsComp = FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer TagContainer;
	TagContainer.Added.AddTag(JujutsuGameplayTags::Cooldown_Dash);
	TargetTagsComp.SetAndApplyTargetTagChanges(TagContainer);

	ApplyCooldownDurationToMagnitude();
}

void UGE_DashCooldown::PostLoad()
{
	Super::PostLoad();

	ApplyCooldownDurationToMagnitude();
}

#if WITH_EDITOR
void UGE_DashCooldown::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UGE_DashCooldown, CooldownDuration))
	{
		ApplyCooldownDurationToMagnitude();
	}
}
#endif

void UGE_DashCooldown::ApplyCooldownDurationToMagnitude()
{
	DurationMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(CooldownDuration));
}
