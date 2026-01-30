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

UENUM(BlueprintType)
enum class EProjectileMoveType : uint8
{
	Single,		// 단발형: 한 번 발사되어 날아가는 타입
	Rush,		// 돌진형: 이동/정지를 반복하며 벽을 무시하는 타입
};
