// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/Abilities/TelePort_Ability.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JujutsuSkillLibrary.h"
#include "Engine/World.h"

void UTelePort_Ability::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AJujutsuBaseCharacter* Character = GetCharacterFromActorInfo();
	if (!Character) return;

	UJujutsuCharacterCombatComponent* Combat = GetCharacterCombatComponentFromActorInfo();
	if (!Combat) return;

	AJujutsuBaseCharacter* Target = Combat->Target.Get();
	if (!Target)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	TArray<FTeleportPositionSlot> Slots = PositionSlots.Num() > 0 ? PositionSlots : GetDefaultSlots();
	// 우선순위 내림차순 정렬
	Slots.Sort([](const FTeleportPositionSlot& A, const FTeleportPositionSlot& B) { return A.Priority > B.Priority; });

	FVector BestPosition = FVector::ZeroVector;
	bool bFoundValid = false;

	for (const FTeleportPositionSlot& Slot : Slots)
	{
		FVector CandidatePos = ComputeWorldPositionFromSlot(Target, Slot);
		FVector ValidPos;
		if (IsPositionValid(Character, CandidatePos, World, ValidPos))
		{
			BestPosition = ValidPos;
			bFoundValid = true;
			break;
		}
	}

	if (!bFoundValid)
	{
		if (bCancelAbilityIfNoValidSlot)
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
			return;
		}
		// 제자리 연출
		BestPosition = Character->GetActorLocation();
	}

	// 이동: SetActorLocation (순간이동)
	Character->SetActorLocation(BestPosition);
	Character->GetCharacterMovement()->StopMovementImmediately();

	// 타겟 방향 회전
	UJujutsuSkillLibrary::SetActorRotationToTarget(Character);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

FVector UTelePort_Ability::ComputeWorldPositionFromSlot(AActor* Target, const FTeleportPositionSlot& Slot) const
{
	if (!Target) return FVector::ZeroVector;

	FVector TargetLoc = Target->GetActorLocation();
	FVector Forward = Target->GetActorForwardVector();
	FVector Right = Target->GetActorRightVector();
	Forward.Z = 0.f;
	Right.Z = 0.f;
	Forward.Normalize();
	Right.Normalize();

	FVector2D Dir = Slot.LocalDirection;
	if (Dir.IsNearlyZero())
	{
		Dir = FVector2D(-1.f, 0.f);
	}
	Dir.Normalize();

	FVector Offset = Forward * Dir.X * DistanceFromTarget + Right * Dir.Y * DistanceFromTarget;
	return TargetLoc + Offset;
}

bool UTelePort_Ability::IsPositionValid(ACharacter* Character, const FVector& CandidatePos, UWorld* World, FVector& OutValidPosition) const
{
	if (!Character || !World) return false;

	UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
	if (!Capsule) return false;

	const float CapsuleRadius = Capsule->GetUnscaledCapsuleRadius();
	const float CapsuleHalfHeight = Capsule->GetUnscaledCapsuleHalfHeight();

	FCollisionQueryParams Params(SCENE_QUERY_STAT(TeleportSweep), false, Character);
	FHitResult SweepHit;
	FQuat Rot = Character->GetActorQuat();
	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);

	// 1) 바닥 존재 확인 (LineTraceDown)
	FVector TraceStart = CandidatePos;
	FVector TraceEnd = TraceStart - FVector(0.f, 0.f, FloorTraceDistance);
	FHitResult FloorHit;
	if (!World->LineTraceSingleByChannel(FloorHit, TraceStart, TraceEnd, ECC_WorldStatic, Params))
	{
		return false;
	}

	// 바닥 위로 보정된 위치
	FVector AdjustedPos = FloorHit.ImpactPoint + FVector(0.f, 0.f, CapsuleHalfHeight);

	// 2) 캡슐 충돌 체크 (Sweep)
	bool bSweepHit = World->SweepSingleByChannel(
		SweepHit,
		AdjustedPos,
		AdjustedPos,
		Rot,
		ECC_Pawn,
		CapsuleShape,
		Params
	);
	if (bSweepHit) return false;

	// 3) MinClearance: 네 방향으로 짧은 라인 트레이스
	if (MinClearance > 0.f)
	{
		const FVector Dirs[] = {
			FVector(1, 0, 0), FVector(-1, 0, 0),
			FVector(0, 1, 0), FVector(0, -1, 0)
		};
		for (const FVector& Dir : Dirs)
		{
			FVector TraceEnd2 = AdjustedPos + Dir * MinClearance;
			FHitResult ClearHit;
			if (World->LineTraceSingleByChannel(ClearHit, AdjustedPos, TraceEnd2, ECC_WorldStatic, Params))
			{
				return false; // 장애물이 너무 가까움
			}
		}
	}

	OutValidPosition = AdjustedPos;
	return true;
}

TArray<FTeleportPositionSlot> UTelePort_Ability::GetDefaultSlots() const
{
	return {
		{ FVector2D(-1.f, 0.f), 1.0f },   // 정면 뒤
		{ FVector2D(-0.8f, 0.5f), 0.8f }, // 뒤-오른쪽
		{ FVector2D(-0.8f, -0.5f), 0.8f }, // 뒤-왼쪽
		{ FVector2D(0.f, 1.f), 0.5f },    // 오른쪽
		{ FVector2D(0.f, -1.f), 0.5f }    // 왼쪽
	};
}
