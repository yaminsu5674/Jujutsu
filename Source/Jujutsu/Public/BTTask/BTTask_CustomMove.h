// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameplayTagContainer.h"
#include "TimerManager.h"
#include "BTTask_CustomMove.generated.h"

class UBlackboardComponent;

struct FBTCustomMoveTaskMemory
{
	bool bWaitingForAbility = false;
	FTimerHandle DashCancelTimerHandle;
};

/**
 * 액션/격투 게임용 이동 태스크.
 * MoveTo 대신 Ability 기반 이동(대시, 점프, 슈퍼점프 등)을 사용.
 * 거리·높이·공중 상태에 따라 적절한 이동 액션을 선택해 실행.
 *
 * 필요한 Blackboard 키:
 * - TargetActor (Object) : 추적할 타겟 액터 (예: 플레이어)
 */
UCLASS()
class JUJUTSU_API UBTTask_CustomMove : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_CustomMove(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	/** 도달로 판정하는 거리 (이 거리 이내면 Succeeded) */
	UPROPERTY(EditAnywhere, Category = "CustomMove", meta = (ClampMin = "0"))
	float AcceptableRadius = 150.f;

	/** 이 거리 이상이면 대시 고려 */
	UPROPERTY(EditAnywhere, Category = "CustomMove|Dash", meta = (ClampMin = "0"))
	float DashMinDistance = 400.f;

	/** 이 거리 이하면 대시 스킵 (너무 가까우면) */
	UPROPERTY(EditAnywhere, Category = "CustomMove|Dash", meta = (ClampMin = "0"))
	float DashMaxDistance = 2000.f;

	/** 대시 시도 확률 (0~1). 매 결정 시 이 확률로 대시 시도 */
	UPROPERTY(EditAnywhere, Category = "CustomMove|Dash", meta = (ClampMin = "0", ClampMax = "1"))
	float DashChance = 0.6f;

	/** AI 전용: 대시를 이 시간(초) 후에 취소. Min~Max 랜덤값 사용 (키 입력 없이 종료시키기 위함) */
	UPROPERTY(EditAnywhere, Category = "CustomMove|Dash", meta = (ClampMin = "0"))
	float DashCancelDurationMin = 0.2f;

	UPROPERTY(EditAnywhere, Category = "CustomMove|Dash", meta = (ClampMin = "0"))
	float DashCancelDurationMax = 0.5f;

	/** 타겟이 이 높이 이상 위에 있으면 점프/슈퍼점프 고려 */
	UPROPERTY(EditAnywhere, Category = "CustomMove|Jump", meta = (ClampMin = "0"))
	float JumpHeightThreshold = 150.f;

	/** 슈퍼점프를 사용할 높이 차이 (이보다 크면 슈퍼점프) */
	UPROPERTY(EditAnywhere, Category = "CustomMove|Jump", meta = (ClampMin = "0"))
	float SuperJumpHeightThreshold = 400.f;

	/** 걷기 이동 입력 강도 */
	UPROPERTY(EditAnywhere, Category = "CustomMove", meta = (ClampMin = "0", ClampMax = "1"))
	float MoveInputStrength = 1.f;

protected:
	/** 이동 Ability 태그 (프로젝트에 맞게 설정) */
	UPROPERTY(EditAnywhere, Category = "CustomMove|Abilities")
	FGameplayTag DashAbilityTag;

	UPROPERTY(EditAnywhere, Category = "CustomMove|Abilities")
	FGameplayTag JumpAbilityTag;

	UPROPERTY(EditAnywhere, Category = "CustomMove|Abilities")
	FGameplayTag SuperJumpAbilityTag;

	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTCustomMoveTaskMemory); }
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const override;

	/** 현재 이동 Ability 활성화 중인지 확인 */
	bool IsMovementAbilityActive(UBehaviorTreeComponent& OwnerComp) const;

	/** 거리·높이·상태에 따라 이동 액션 결정 및 실행 */
	EBTNodeResult::Type ExecuteMoveDecision(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	/** AI 전용: 랜덤 시간 후 대시 Ability 취소 (키 입력 없이 종료) */
	void ScheduleDashCancel(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
