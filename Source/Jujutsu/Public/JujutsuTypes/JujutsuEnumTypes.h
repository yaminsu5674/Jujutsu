// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "JujutsuEnumTypes.generated.h"

UENUM(BlueprintType)
enum class EJujutsuAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

UENUM(BlueprintType)
enum class EJujutsuSuccessType : uint8
{
	Successful,
	Failed
};
