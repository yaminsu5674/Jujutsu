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
