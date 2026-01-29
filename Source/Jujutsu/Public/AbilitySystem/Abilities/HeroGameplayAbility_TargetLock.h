// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/JujutsuGameplayAbility.h"
#include "HeroGameplayAbility_TargetLock.generated.h"

/**
 * 전투 컴포넌트의 Target을 바라보게 하는 어빌리티.
 * 타겟 검색 없이 CombatComponent->Target만 사용하며, 타겟이 있으면 캐릭터/컨트롤러 회전만 갱신.
 */
UCLASS()
class JUJUTSU_API UHeroGameplayAbility_TargetLock : public UJujutsuGameplayAbility
{
	GENERATED_BODY()

public:
	UHeroGameplayAbility_TargetLock();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/** 매 틱: CombatComponent->Target 확인 후 타겟이 있으면 바라보게 회전 */
	void OnTargetLockTick();

	/** 카메라 피치 오프셋 (세로, 위·아래. 양수=아래로) */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|TargetLock")
	float TargetLockCameraPitchOffset = 0.f;

	/** 카메라 요 오프셋 (가로, 좌·우. 양수=오른쪽) */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|TargetLock")
	float TargetLockCameraYawOffset = 0.f;

	/** 회전 보간 속도 */
	UPROPERTY(EditDefaultsOnly, Category = "Jujutsu|TargetLock", meta = (ClampMin = "0.1"))
	float TargetLockRotationInterpSpeed = 10.f;

	FTimerHandle TargetLockTickTimerHandle;
};
