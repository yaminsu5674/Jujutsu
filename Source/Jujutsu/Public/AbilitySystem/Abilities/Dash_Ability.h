// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/JujutsuGameplayAbility.h"
#include "Animation/AnimMontage.h"
#include "Dash_Ability.generated.h"

struct FTimerHandle;

/**
 * 대시 어빌리티.
 */
UCLASS()
class JUJUTSU_API UDash_Ability : public UJujutsuGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Dash")
	UAnimMontage* DashMontage = nullptr;

	/** 공중 대시 시 RunSpeed에 곱하는 배율. 낮추면 덜 급격하게 날아감 */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Dash", meta = (ClampMin = "0"))
	float AirDashSpeedMultiplier = 2.f;

	/** 지상 대시 시 RunSpeed에 곱하는 배율 */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Dash", meta = (ClampMin = "0"))
	float GroundDashSpeedMultiplier = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Dash", meta = (ClampMin = "0"))
	float AirDashGravityRestoreDelay = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|Dash", meta = (ClampMin = "0"))
	float GroundDashFrictionRestoreDelay = 0.3f;

	/** 공중 대시 시 중력 복원 타이머 */
	FTimerHandle DashGravityRestoreHandle;
	/** 지상 대시 시 마찰 복원 타이머 */
	FTimerHandle DashFrictionRestoreHandle;

	/** 캐시: 대시 적용 전 캐릭터 무브먼트 원래 값 (복원용) */
	float SavedGravityScale = 1.f;
	float SavedBrakingFrictionFactor = 2.f;
	float SavedGroundFriction = 8.f;
	float SavedBrakingDecelerationWalking = 2000.f;

	/** 공중/지상 판단·이동 여부는 애님 인스턴스·이동속도 XY 사용. 대시 전 무브먼트 세팅 및 대시 실행 */
	void ApplyMovementForAirDash(AJujutsuBaseCharacter* Character);
	void ApplyMovementForGroundDash(AJujutsuBaseCharacter* Character);
	/** 타이머 콜백 또는 EndAbility에서 호출. 복원 후 플래그 클리어 */
	void RestoreMovementAfterAirDash();
	void RestoreMovementAfterGroundDash();

	/** 대시가 온전히 끝났는지. true일 때만 EndAbility에서 실제 종료 */
	bool bDashFinished = false;

	/** EndAbility가 이미 호출됐는지(키 뗐는지). Restore 끝에서 true면 그때 EndAbility 수행 */
	bool bEndAbilityRequested = false;

	/** 타이머 콜백에서 EndAbility 호출 시 사용 */
	FGameplayAbilitySpecHandle CachedAbilityHandle;
	const FGameplayAbilityActorInfo* CachedActorInfo = nullptr;
	FGameplayAbilityActivationInfo CachedActivationInfo;

	bool bIsAirDashing = false;
	bool bIsGroundDashing = false;
};
