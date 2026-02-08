// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GE_DashCooldown.generated.h"

/**
 * 대시(Dash) 어빌리티 쿨다운용 GameplayEffect.
 * Granted Tags: Cooldown.Dash
 * Dash 어빌리티의 Cooldown Gameplay Effect Class로 지정하고,
 * Activation Blocked Tags에 Cooldown.Dash를 등록하여 사용.
 */
UCLASS()
class JUJUTSU_API UGE_DashCooldown : public UGameplayEffect
{
	GENERATED_BODY()

public:
	UGE_DashCooldown(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitProperties() override;
	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/** 쿨다운 지속 시간 (초). 블루프린트에서 수정 가능. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dash Cooldown", meta = (ClampMin = "0.1"))
	float CooldownDuration = 3.0f;

private:
	void ApplyCooldownDurationToMagnitude();
};
