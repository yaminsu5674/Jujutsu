// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JujutsuCharacterMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class JUJUTSU_API UJujutsuCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData|Movement", meta = (ClampMin = "0"))
	float WalkSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData|Movement", meta = (ClampMin = "0"))
	float RunSpeed = 600.f;

	/** 부모 GravityScale에 적용되는 기본값. 생성자에서 적용. 스킬 등에서 중력 껐다 켤 때 복원값으로 사용 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData|Movement", meta = (ClampMin = "0"))
	float DefaultGravityScale = 2.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData|Movement", meta = (ClampMin = "0"))
	int32 JumpCount = 0;

	UJujutsuCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
