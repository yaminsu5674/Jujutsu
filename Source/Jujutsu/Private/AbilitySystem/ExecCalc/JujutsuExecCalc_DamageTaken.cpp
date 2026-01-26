// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/ExecCalc/JujutsuExecCalc_DamageTaken.h"
#include "AbilitySystem/JujutsuAttributeSet.h"
#include "JujutsuGameplayTags.h"

struct FJujutsuDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)

	FJujutsuDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UJujutsuAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UJujutsuAttributeSet, DefensePower, Target, false)
	}
};

static const FJujutsuDamageCapture& GetJujutsuDamageCapture()
{
	static FJujutsuDamageCapture JujutsuDamageCapture;
	return JujutsuDamageCapture;
}

UJujutsuExecCalc_DamageTaken::UJujutsuExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetJujutsuDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetJujutsuDamageCapture().DefensePowerDef);
}

void UJujutsuExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetJujutsuDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);

	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(JujutsuGameplayTags::Character_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}

		if (TagMagnitude.Key.MatchesTagExact(JujutsuGameplayTags::Character_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = static_cast<int32>(TagMagnitude.Value);
		}

		if (TagMagnitude.Key.MatchesTagExact(JujutsuGameplayTags::Character_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = static_cast<int32>(TagMagnitude.Value);
		}
	}

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetJujutsuDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
}
