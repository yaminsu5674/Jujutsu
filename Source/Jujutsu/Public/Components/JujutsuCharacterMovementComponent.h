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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterData|Movement", meta = (ClampMin = "0"))
	int32 JumpCount = 0;

	UJujutsuCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
