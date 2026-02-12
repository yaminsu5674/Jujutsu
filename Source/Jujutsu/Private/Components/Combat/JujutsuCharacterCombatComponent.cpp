// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "JujutsuGameplayTags.h"
#include "JujutsuSkillLibrary.h"
#include "Net/UnrealNetwork.h"

UJujutsuCharacterCombatComponent::UJujutsuCharacterCombatComponent()
{
	HitEventTag = JujutsuGameplayTags::Character_Event_Hit_Light;
	SetIsReplicatedByDefault(true);
}

void UJujutsuCharacterCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UJujutsuCharacterCombatComponent, ReplicatedTarget);
}

void UJujutsuCharacterCombatComponent::OnRep_Target()
{
	Target = ReplicatedTarget;
}

void UJujutsuCharacterCombatComponent::SetTarget(AJujutsuBaseCharacter* NewTarget)
{
	ReplicatedTarget = NewTarget;
	Target = NewTarget;
}

void UJujutsuCharacterCombatComponent::SetHitEventTag(FGameplayTag InTag)
{
	HitEventTag = InTag;
}

void UJujutsuCharacterCombatComponent::SetBaseDamage(float InDamage)
{
	Damage = FMath::Max(0.f, InDamage);
}

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
	ToggleBodyCollisionParts(bShouldEnable, TArray<EBodyCollisionPart>());
}

void UJujutsuCharacterCombatComponent::ToggleBodyCollisionParts(bool bShouldEnable, const TArray<EBodyCollisionPart>& Parts)
{
	AJujutsuBaseCharacter* BaseCharacter = GetOwningPawn<AJujutsuBaseCharacter>();
	if (!BaseCharacter) return;

	const bool bApplyToAll = Parts.Num() == 0;

	auto ShouldEnablePart = [&Parts, bApplyToAll](EBodyCollisionPart Part) -> bool
	{
		return bApplyToAll || Parts.Contains(Part);
	};

	auto ShouldDisablePart = [&Parts, bApplyToAll](EBodyCollisionPart Part) -> bool
	{
		return bApplyToAll || Parts.Contains(Part);
	};

	if (bShouldEnable)
	{
		const ECollisionEnabled::Type EnableState = ECollisionEnabled::QueryOnly;
		const ECollisionEnabled::Type DisableState = ECollisionEnabled::NoCollision;
		if (UBoxComponent* Box = BaseCharacter->GetLeftHandCollisionBox())
			Box->SetCollisionEnabled(ShouldEnablePart(EBodyCollisionPart::LeftHand) ? EnableState : DisableState);
		if (UBoxComponent* Box = BaseCharacter->GetRightHandCollisionBox())
			Box->SetCollisionEnabled(ShouldEnablePart(EBodyCollisionPart::RightHand) ? EnableState : DisableState);
		if (UBoxComponent* Box = BaseCharacter->GetLeftFootCollisionBox())
			Box->SetCollisionEnabled(ShouldEnablePart(EBodyCollisionPart::LeftFoot) ? EnableState : DisableState);
		if (UBoxComponent* Box = BaseCharacter->GetRightFootCollisionBox())
			Box->SetCollisionEnabled(ShouldEnablePart(EBodyCollisionPart::RightFoot) ? EnableState : DisableState);
	}
	else
	{
		const ECollisionEnabled::Type DisableState = ECollisionEnabled::NoCollision;
		if (UBoxComponent* Box = BaseCharacter->GetLeftHandCollisionBox())
			if (ShouldDisablePart(EBodyCollisionPart::LeftHand)) Box->SetCollisionEnabled(DisableState);
		if (UBoxComponent* Box = BaseCharacter->GetRightHandCollisionBox())
			if (ShouldDisablePart(EBodyCollisionPart::RightHand)) Box->SetCollisionEnabled(DisableState);
		if (UBoxComponent* Box = BaseCharacter->GetLeftFootCollisionBox())
			if (ShouldDisablePart(EBodyCollisionPart::LeftFoot)) Box->SetCollisionEnabled(DisableState);
		if (UBoxComponent* Box = BaseCharacter->GetRightFootCollisionBox())
			if (ShouldDisablePart(EBodyCollisionPart::RightFoot)) Box->SetCollisionEnabled(DisableState);
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
	Data.Instigator = OwningPawn;  // 때린 쪽
	Data.Target = HitActor;        // 맞은 쪽

	// 공격자에게만 HitSuccess 전달 → 블루프린트에서 이 이벤트 대기 후 Data.Target에게 피격반응/데미지 적용
	// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
	// 	OwningPawn,
	// 	JujutsuGameplayTags::Character_Event_HitSuccess,
	// 	Data
	// );

	// 피격자에게 HitEventTag 전달
	if (HitEventTag.IsValid())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, HitEventTag, Data);
	}

	// 데미지 적용
	if (DamageEffectClass)
	{
		UJujutsuSkillLibrary::ApplyDamageEffectToTarget(
			HitActor,
			DamageEffectClass,
			Damage,
			0,   // UsedComboCount (바디 콜리전은 0)
			1,   // Level
			OwningPawn // InstigatorForContext (공격자)
		);
	}
}

void UJujutsuCharacterCombatComponent::OnPulledFromTargetActor(AActor* InteractedActor)
{
}
