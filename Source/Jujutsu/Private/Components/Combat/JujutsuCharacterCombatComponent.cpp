// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "JujutsuDebugHelper.h"

void UJujutsuCharacterCombatComponent::ToggleBodyCollision(bool bShouldEnable)
{
	AJujutsuBaseCharacter* BaseCharacter = GetOwningPawn<AJujutsuBaseCharacter>();
	if (!BaseCharacter) return;

	const ECollisionEnabled::Type NewState = bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;

	if (UBoxComponent* Box = BaseCharacter->GetLeftHandCollisionBox())  { Box->SetCollisionEnabled(NewState); }
	if (UBoxComponent* Box = BaseCharacter->GetRightHandCollisionBox()) { Box->SetCollisionEnabled(NewState); }
	if (UBoxComponent* Box = BaseCharacter->GetLeftFootCollisionBox())  { Box->SetCollisionEnabled(NewState); }
	if (UBoxComponent* Box = BaseCharacter->GetRightFootCollisionBox()) { Box->SetCollisionEnabled(NewState); }
}
