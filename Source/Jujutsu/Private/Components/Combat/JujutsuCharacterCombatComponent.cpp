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

	APawn* OwningPawn = GetOwningPawn();
	if (!OwningPawn) return;

	// 로컬 플레이어(클라/리슨서버 본인)이거나 서버가 조종하는 AI일 때만 판정 시작
	const bool bIsLocalPlayer = OwningPawn->IsLocallyControlled();
	const bool bIsServerAI = OwningPawn->HasAuthority() && !OwningPawn->IsPlayerControlled();

	if (bIsLocalPlayer || bIsServerAI)
	{
		if (OverlappedActors.Contains(HitActor)) return;
		OverlappedActors.AddUnique(HitActor);
		Server_ProcessHit(HitActor, HitEventTag);
	}
}

void UJujutsuCharacterCombatComponent::Server_ProcessHit_Implementation(AActor* HitActor, FGameplayTag PassedHitTag)
{
	if (!HitActor) return;

	APawn* OwningPawn = GetOwningPawn();
	if (!OwningPawn) return;

	// 원격 클라이언트가 보낸 RPC일 때만 서버에서 중복 체크 (서버 본인/AI가 때린 건 OnHitTargetActor에서 이미 추가됨)
	if (!OwningPawn->IsLocallyControlled())
	{
		if (OverlappedActors.Contains(HitActor)) return;
		OverlappedActors.AddUnique(HitActor);
	}

	FGameplayEventData Data;
	Data.Instigator = OwningPawn;
	Data.Target = HitActor;

	if (PassedHitTag.IsValid())
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, PassedHitTag, Data);
	}

	if (DamageEffectClass)
	{
		UJujutsuSkillLibrary::ApplyDamageEffectToTarget(
			HitActor,
			DamageEffectClass,
			Damage,
			0,
			1,
			OwningPawn
		);
	}
}

void UJujutsuCharacterCombatComponent::OnPulledFromTargetActor(AActor* InteractedActor)
{
}
