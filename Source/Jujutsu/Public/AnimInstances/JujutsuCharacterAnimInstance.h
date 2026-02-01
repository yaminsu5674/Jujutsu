// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/JujutsuBaseAnimInstance.h"
#include "GameplayTagContainer.h"
#include "JujutsuCharacterAnimInstance.generated.h"

class AJujutsuBaseCharacter;
class UCharacterMovementComponent;

/**
 *
 */
UCLASS()
class JUJUTSU_API UJujutsuCharacterAnimInstance : public UJujutsuBaseAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	AJujutsuBaseCharacter* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float VerticalSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bIsFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimData|State")
	bool bIsHit = false;

	UFUNCTION(BlueprintCallable, Category = "AnimData|GameplayTags")
	bool HasGameplayTag(FGameplayTag Tag) const;
};
