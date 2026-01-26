// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "JujutsuExecCalc_DamageTaken.generated.h"

/**
 * 데미지 산출용 Execution Calculation
 * GE에서 SetByCaller(BaseDamage, AttackType 등)를 읽어 최종 데미지를 계산할 때 사용
 */
UCLASS()
class JUJUTSU_API UJujutsuExecCalc_DamageTaken : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UJujutsuExecCalc_DamageTaken();
};
