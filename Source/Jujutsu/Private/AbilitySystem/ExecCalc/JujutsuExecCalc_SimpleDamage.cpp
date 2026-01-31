// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/ExecCalc/JujutsuExecCalc_SimpleDamage.h"
#include "AbilitySystem/JujutsuAttributeSet.h"
#include "JujutsuGameplayTags.h"

struct FJujutsuSimpleDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FJujutsuSimpleDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UJujutsuAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UJujutsuAttributeSet, DamageTaken, Target, false)
	}
};

static const FJujutsuSimpleDamageCapture& GetJujutsuSimpleDamageCapture()
{
	static FJujutsuSimpleDamageCapture JujutsuSimpleDamageCapture;
	return JujutsuSimpleDamageCapture;
}

UJujutsuExecCalc_SimpleDamage::UJujutsuExecCalc_SimpleDamage()
{
	RelevantAttributesToCapture.Add(GetJujutsuSimpleDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetJujutsuSimpleDamageCapture().DamageTakenDef);
}

void UJujutsuExecCalc_SimpleDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float BaseDamage = 0.f;
	int32 UsedComboCount = 0;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(JujutsuGameplayTags::Character_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}
		if (TagMagnitude.Key.MatchesTagExact(JujutsuGameplayTags::Character_SetByCaller_UsedComboCount))
		{
			UsedComboCount = static_cast<int32>(TagMagnitude.Value);
		}
	}

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetJujutsuSimpleDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);

	if (UsedComboCount > 0)
	{
		const float DamageIncreasePercent = (UsedComboCount - 1) * 0.05f + 1.f;
		BaseDamage *= DamageIncreasePercent;
	}

	const float FinalDamageDone = BaseDamage / FMath::Max(TargetDefensePower, 1.f);

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetJujutsuSimpleDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
