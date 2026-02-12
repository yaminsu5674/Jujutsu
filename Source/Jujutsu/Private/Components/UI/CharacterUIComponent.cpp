// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/UI/CharacterUIComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "AbilitySystem/JujutsuAttributeSet.h"

void UCharacterUIComponent::BroadcastCurrentHealthAndRage()
{
	AJujutsuBaseCharacter* Char = GetOwner<AJujutsuBaseCharacter>();
	const UJujutsuAttributeSet* Attr = Char ? Char->GetJujutsuAttributeSet() : nullptr;
	if (!Attr) return;

	const float MaxH = Attr->GetMaxHealth();
	const float MaxR = Attr->GetMaxRage();
	if (MaxH > 0.f) OnCurrentHealthChanged.Broadcast(Attr->GetCurrentHealth() / MaxH);
	if (MaxR > 0.f) OnCurrentRageChanged.Broadcast(Attr->GetCurrentRage() / MaxR);
}
