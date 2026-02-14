// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "JujutsuEnumTypes.generated.h"

UENUM(BlueprintType)
enum class EJujutsuAbilityInputType : uint8
{
	Pressed,	// 키 누르면 활성화, 한 번 실행
	Keep		// 키 누르고 있는 동안 활성, 키 떼면 EndAbility
};

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

UENUM(BlueprintType)
enum class EPushMode : uint8
{
	AttachToSource,
	Impulse
};

UENUM(BlueprintType)
enum class EBodyCollisionPart : uint8
{
	LeftHand,
	RightHand,
	LeftFoot,
	RightFoot
};

/** 텔레포트 기본 슬롯 검사 시 어느 방향을 우선할지. 자식 어빌리티에서 EditDefaultsOnly로 변경 가능 */
UENUM(BlueprintType)
enum class ETeleportPriorityDirection : uint8
{
	Back   UMETA(DisplayName = "Back"),
	Right  UMETA(DisplayName = "Right"),
	Left   UMETA(DisplayName = "Left")
};
