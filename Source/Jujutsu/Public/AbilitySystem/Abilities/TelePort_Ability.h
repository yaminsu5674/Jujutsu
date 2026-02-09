// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/JujutsuGameplayAbility.h"
#include "TelePort_Ability.generated.h"

/**
 * 타겟 기준 상대 위치 슬롯. 우선순위 높은 순으로 검사.
 * LocalDirection: Target 로컬 (X=앞뒤, -1=뒤 / Y=좌우). 정규화 권장.
 */
USTRUCT(BlueprintType)
struct FTeleportPositionSlot
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Teleport Slot", meta = (DisplayName = "Local Direction (X=Back/Fwd, Y=Left/Right)"))
	FVector2D LocalDirection = FVector2D(-1.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "Teleport Slot", meta = (ClampMin = "0", ClampMax = "1"))
	float Priority = 1.f;
};

/**
 * Combat Teleport Position Resolver.
 * 타겟(CombatComponent.Target) 기준으로 최적의 텔레포트 위치를 계산하여 이동.
 * 뒤 → 옆 → 대각 순으로 검사하며, 캡슐 충돌/바닥/벽 간격 체크.
 * 이벤트(Character_Event_Teleport)로만 활성화. Ability Trigger에 Character_Event_Teleport 등록.
 */
UCLASS()
class JUJUTSU_API UTelePort_Ability : public UJujutsuGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** 타겟과 텔레포트할 떨어진 거리 (월드 유닛) */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Teleport", meta = (ClampMin = "1"))
	float DistanceFromTarget = 150.f;

	/** 위치 슬롯 (우선순위 순 정렬 후 검사). 비어 있으면 기본 슬롯 사용 */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Teleport")
	TArray<FTeleportPositionSlot> PositionSlots;

	/** 벽/장애물과의 최소 간격 (이보다 가까우면 해당 슬롯 불가) */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Teleport", meta = (ClampMin = "0"))
	float MinClearance = 30.f;

	/** 바닥 검사 LineTrace 거리 */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Teleport", meta = (ClampMin = "10"))
	float FloorTraceDistance = 200.f;

	/** 유효 슬롯 없으면 어빌리티 취소 (false면 제자리) */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Teleport")
	bool bCancelAbilityIfNoValidSlot = true;

	/** 타겟 기준 월드 좌표 계산 */
	FVector ComputeWorldPositionFromSlot(AActor* Target, const FTeleportPositionSlot& Slot) const;

	/** 해당 위치가 유효한지 검사. 유효하면 OutValidPosition에 바닥 보정된 위치 반환 */
	bool IsPositionValid(ACharacter* Character, const FVector& CandidatePos, UWorld* World, FVector& OutValidPosition) const;

	/** 기본 슬롯 반환 (뒤 → 뒤-오른쪽 → 뒤-왼쪽 → 오른쪽 → 왼쪽) */
	TArray<FTeleportPositionSlot> GetDefaultSlots() const;
};
