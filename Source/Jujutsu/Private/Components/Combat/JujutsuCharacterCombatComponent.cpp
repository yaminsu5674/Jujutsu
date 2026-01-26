// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "JujutsuGameplayTags.h"

void UJujutsuCharacterCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AJujutsuBaseCharacter* BaseCharacter = GetOwningPawn<AJujutsuBaseCharacter>())
	{
		BaseCharacter->OnBodyHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
		BaseCharacter->OnBodyPulledFromTarget.BindUObject(this, &ThisClass::OnPulledFromTargetActor);
	}
}

void UJujutsuCharacterCombatComponent::ToggleBodyCollision(bool bShouldEnable)
{
	AJujutsuBaseCharacter* BaseCharacter = GetOwningPawn<AJujutsuBaseCharacter>();
	if (!BaseCharacter) return;

	const ECollisionEnabled::Type NewState = bShouldEnable ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;

	if (UBoxComponent* Box = BaseCharacter->GetLeftHandCollisionBox())  { Box->SetCollisionEnabled(NewState); }
	if (UBoxComponent* Box = BaseCharacter->GetRightHandCollisionBox()) { Box->SetCollisionEnabled(NewState); }
	if (UBoxComponent* Box = BaseCharacter->GetLeftFootCollisionBox())  { Box->SetCollisionEnabled(NewState); }
	if (UBoxComponent* Box = BaseCharacter->GetRightFootCollisionBox()) { Box->SetCollisionEnabled(NewState); }

	if (!bShouldEnable)
	{
		OverlappedActors.Empty();
	}
}

void UJujutsuCharacterCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	if (!HitActor) return;

	if (OverlappedActors.Contains(HitActor))
	{
		return;
	}

	OverlappedActors.AddUnique(HitActor);

	APawn* OwningPawn = GetOwningPawn();
	if (!OwningPawn) return;

	FGameplayEventData Data;
	Data.Instigator = OwningPawn;
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		OwningPawn,
		JujutsuGameplayTags::Character_Event_Hit,
		Data
	);
}

void UJujutsuCharacterCombatComponent::OnPulledFromTargetActor(AActor* InteractedActor)
{
}
