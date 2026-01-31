// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "JujutsuExecCalc_SimpleDamage.generated.h"

/**
 * 단순 데미지 Execution Calculation
 * Source(공격자) 속성은 캡처하지 않고, SetByCaller(BaseDamage, UsedComboCount)와 Target(DefensePower)만 사용.
 * 발사체 등 Source ASC가 없을 때 사용.
 */
UCLASS()
class JUJUTSU_API UJujutsuExecCalc_SimpleDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UJujutsuExecCalc_SimpleDamage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
