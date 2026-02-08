// Copyright Epic Games, Inc. All Rights Reserved.

#include "BTTask/BTTask_CustomMove.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "AbilitySystem/JujutsuAbilitySystemComponent.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "JujutsuGameplayTags.h"
#include "JujutsuSkillLibrary.h"
#include "TimerManager.h"

UBTTask_CustomMove::UBTTask_CustomMove(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Custom Move";
	INIT_TASK_NODE_NOTIFY_FLAGS();

	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_CustomMove, BlackboardKey), AActor::StaticClass());

	DashAbilityTag = JujutsuGameplayTags::Character_Ability_Move_Dash;
	JumpAbilityTag = JujutsuGameplayTags::Character_Ability_Move_Jump;
	SuperJumpAbilityTag = JujutsuGameplayTags::Character_Ability_Move_SuperJump;
}

void UBTTask_CustomMove::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = Asset.BlackboardAsset)
	{
		BlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTTask_CustomMove::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{
	Super::InitializeMemory(OwnerComp, NodeMemory, InitType);
	FBTCustomMoveTaskMemory* Mem = reinterpret_cast<FBTCustomMoveTaskMemory*>(NodeMemory);
	if (Mem)
	{
		Mem->bWaitingForAbility = false;
		Mem->DashCancelTimerHandle.Invalidate();
	}
}

void UBTTask_CustomMove::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type CleanupType) const
{
	FBTCustomMoveTaskMemory* Mem = reinterpret_cast<FBTCustomMoveTaskMemory*>(NodeMemory);
	if (Mem && Mem->DashCancelTimerHandle.IsValid())
	{
		if (UWorld* World = OwnerComp.GetWorld())
		{
			World->GetTimerManager().ClearTimer(Mem->DashCancelTimerHandle);
		}
		Mem->DashCancelTimerHandle.Invalidate();
	}
	Super::CleanupMemory(OwnerComp, NodeMemory, CleanupType);
}

bool UBTTask_CustomMove::IsMovementAbilityActive(UBehaviorTreeComponent& OwnerComp) const
{
	APawn* Pawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	AJujutsuBaseCharacter* Character = Cast<AJujutsuBaseCharacter>(Pawn);
	if (!Character) return false;

	UJujutsuAbilitySystemComponent* ASC = Character->GetJujutsuAbilitySystemComponent();
	if (!ASC) return false;

	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (!Spec.IsActive()) continue;

		UGameplayAbility* Ability = Spec.Ability;
		if (!Ability) continue;

		if (Ability->GetAssetTags().HasTag(JujutsuGameplayTags::Character_Ability_Move))
		{
			return true;
		}
	}

	return false;
}

void UBTTask_CustomMove::ScheduleDashCancel(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	AJujutsuBaseCharacter* Character = Cast<AJujutsuBaseCharacter>(Pawn);
	if (!Character) return;

	UWorld* World = Character->GetWorld();
	if (!World) return;

	FBTCustomMoveTaskMemory* Mem = reinterpret_cast<FBTCustomMoveTaskMemory*>(NodeMemory);
	if (Mem && Mem->DashCancelTimerHandle.IsValid())
	{
		World->GetTimerManager().ClearTimer(Mem->DashCancelTimerHandle);
	}

	const float Duration = FMath::RandRange(DashCancelDurationMin, DashCancelDurationMax);
	FGameplayTag TagToCancel = DashAbilityTag;

	if (Mem)
	{
		World->GetTimerManager().SetTimer(Mem->DashCancelTimerHandle, [Character, TagToCancel, NodeMemory]()
		{
			UJujutsuAbilitySystemComponent* ASC = Character ? Character->GetJujutsuAbilitySystemComponent() : nullptr;
			if (ASC && TagToCancel.IsValid())
			{
				FGameplayTagContainer CancelTags;
				CancelTags.AddTag(TagToCancel);
				ASC->CancelAbilities(&CancelTags, nullptr, nullptr);
			}

			FBTCustomMoveTaskMemory* MemPtr = reinterpret_cast<FBTCustomMoveTaskMemory*>(NodeMemory);
			if (MemPtr)
			{
				MemPtr->DashCancelTimerHandle.Invalidate();
				MemPtr->bWaitingForAbility = false;
			}
		}, Duration, false);
	}
}

EBTNodeResult::Type UBTTask_CustomMove::ExecuteMoveDecision(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Pawn = AIController ? AIController->GetPawn() : nullptr;
	AJujutsuBaseCharacter* Character = Cast<AJujutsuBaseCharacter>(Pawn);
	if (!Character)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(BlackboardKey.SelectedKeyName));
	AJujutsuBaseCharacter* TargetCharacter = Cast<AJujutsuBaseCharacter>(TargetActor);
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	if (UJujutsuCharacterCombatComponent* Combat = Character->GetCharacterCombatComponent())
	{
		Combat->Target = TargetCharacter;
	}

	const FVector MyLoc = Character->GetActorLocation();
	const FVector TargetLoc = TargetActor->GetActorLocation();
	const float DistXY = FVector::Dist2D(MyLoc, TargetLoc);
	const float HeightDiff = TargetLoc.Z - MyLoc.Z;

	const bool bTargetInAir = TargetCharacter && TargetCharacter->GetCharacterMovement() && TargetCharacter->GetCharacterMovement()->IsFalling();
	const bool bSelfInAir = Character->GetCharacterMovement() && Character->GetCharacterMovement()->IsFalling();

	if (DistXY < AcceptableRadius && FMath::Abs(HeightDiff) < AcceptableRadius * 0.5f)
	{
		return EBTNodeResult::Succeeded;
	}

	UJujutsuSkillLibrary::SetActorRotationToTarget(Character);

	UJujutsuAbilitySystemComponent* ASC = Character->GetJujutsuAbilitySystemComponent();
	if (!ASC) return EBTNodeResult::Failed;

	// 1) 타겟이 공중에 있고, 자신은 지상에 있음 → 점프로 추격
	if (bTargetInAir && !bSelfInAir && HeightDiff > JumpHeightThreshold)
	{
		FGameplayTag JumpTag = HeightDiff > SuperJumpHeightThreshold ? SuperJumpAbilityTag : JumpAbilityTag;
		if (JumpTag.IsValid() && ASC->TryActivateAbilityByTag(JumpTag))
		{
			FBTCustomMoveTaskMemory* Mem = reinterpret_cast<FBTCustomMoveTaskMemory*>(NodeMemory);
			if (Mem) Mem->bWaitingForAbility = true;
			return EBTNodeResult::InProgress;
		}
	}

	// 2) 공중에 있음 → 이동 입력만
	if (bSelfInAir)
	{
		Character->AddMovementInput(Character->GetActorForwardVector(), MoveInputStrength);
		return EBTNodeResult::InProgress;
	}

	// 3) 거리 조건 충족 시 대시
	if (DistXY >= DashMinDistance && DistXY <= DashMaxDistance && FMath::FRand() < DashChance)
	{
		if (DashAbilityTag.IsValid() && ASC->TryActivateAbilityByTag(DashAbilityTag))
		{
			FBTCustomMoveTaskMemory* Mem = reinterpret_cast<FBTCustomMoveTaskMemory*>(NodeMemory);
			if (Mem)
			{
				Mem->bWaitingForAbility = true;
			}
			ScheduleDashCancel(OwnerComp, NodeMemory);
			return EBTNodeResult::InProgress;
		}
	}

	// 4) 기본: 전진 이동
	Character->AddMovementInput(Character->GetActorForwardVector(), MoveInputStrength);
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_CustomMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTCustomMoveTaskMemory* Mem = reinterpret_cast<FBTCustomMoveTaskMemory*>(NodeMemory);
	if (Mem)
	{
		Mem->bWaitingForAbility = false;
		Mem->DashCancelTimerHandle.Invalidate();
	}

	return ExecuteMoveDecision(OwnerComp, NodeMemory);
}

void UBTTask_CustomMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBTCustomMoveTaskMemory* Mem = reinterpret_cast<FBTCustomMoveTaskMemory*>(NodeMemory);
	APawn* Pawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	AJujutsuBaseCharacter* Character = Cast<AJujutsuBaseCharacter>(Pawn);

	if (Mem && Mem->bWaitingForAbility)
	{
		// 대시/점프 대기 중에도 이동 입력 적용 (대시 시 속도 차이, 걷기처럼 이동)
		if (Character && !Character->GetCharacterMovement()->IsFalling())
		{
			Character->AddMovementInput(Character->GetActorForwardVector(), MoveInputStrength);
		}

		if (!IsMovementAbilityActive(OwnerComp))
		{
			Mem->bWaitingForAbility = false;
			if (Mem->DashCancelTimerHandle.IsValid() && (Character && Character->GetWorld()))
			{
				Character->GetWorld()->GetTimerManager().ClearTimer(Mem->DashCancelTimerHandle);
				Mem->DashCancelTimerHandle.Invalidate();
			}
		}
		else
		{
			return;
		}
	}

	EBTNodeResult::Type Result = ExecuteMoveDecision(OwnerComp, NodeMemory);

	if (Result == EBTNodeResult::Succeeded)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (Result == EBTNodeResult::Failed)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		if (Character && !Character->GetCharacterMovement()->IsFalling())
		{
			Character->AddMovementInput(Character->GetActorForwardVector(), MoveInputStrength);
		}
	}
}

EBTNodeResult::Type UBTTask_CustomMove::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTCustomMoveTaskMemory* Mem = reinterpret_cast<FBTCustomMoveTaskMemory*>(NodeMemory);
	if (Mem)
	{
		if (Mem->DashCancelTimerHandle.IsValid())
		{
			if (UWorld* World = OwnerComp.GetWorld())
			{
				World->GetTimerManager().ClearTimer(Mem->DashCancelTimerHandle);
			}
			Mem->DashCancelTimerHandle.Invalidate();
		}
		Mem->bWaitingForAbility = false;
	}

	return EBTNodeResult::Aborted;
}

FString UBTTask_CustomMove::GetStaticDescription() const
{
	return FString::Printf(TEXT("Custom Move (Target: %s, Radius: %.0f)"), *BlackboardKey.SelectedKeyName.ToString(), AcceptableRadius);
}
