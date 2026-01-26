// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystem/ExecCalc/JujutsuExecCalc_DamageTaken.h"
#include "AbilitySystem/JujutsuAttributeSet.h"
#include "JujutsuGameplayTags.h"
#include "JujutsuDebugHelper.h"

struct FJujutsuDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FJujutsuDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UJujutsuAttributeSet, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UJujutsuAttributeSet, DefensePower, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UJujutsuAttributeSet, DamageTaken, Target, false)
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
	RelevantAttributesToCapture.Add(GetJujutsuDamageCapture().DamageTakenDef);
}

void UJujutsuExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetJujutsuDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	/* Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower); */

	float BaseDamage = 0.f;
	int32 UsedComboCount = 0;

	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(JujutsuGameplayTags::Character_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			/* Debug::Print(TEXT("BaseDamage"), BaseDamage); */
		}

		if (TagMagnitude.Key.MatchesTagExact(JujutsuGameplayTags::Character_SetByCaller_UsedComboCount))
		{
			UsedComboCount = static_cast<int32>(TagMagnitude.Value);
			/* Debug::Print(TEXT("UsedComboCount"), static_cast<float>(UsedComboCount)); */
		}
	}

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetJujutsuDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	/* Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower); */

	if (UsedComboCount > 0)
	{
		const float DamageIncreasePercent = (UsedComboCount - 1) * 0.05f + 1.f;
		BaseDamage *= DamageIncreasePercent;
		/* Debug::Print(TEXT("ScaledBaseDamage"), BaseDamage); */
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / FMath::Max(TargetDefensePower, 1.f);
	/* Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone); */

	if (FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetJujutsuDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
