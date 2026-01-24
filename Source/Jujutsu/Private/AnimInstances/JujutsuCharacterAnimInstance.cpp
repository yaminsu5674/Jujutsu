// Copyright Epic Games, Inc. All Rights Reserved.



#include "AnimInstances/JujutsuCharacterAnimInstance.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UJujutsuCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<AJujutsuBaseCharacter>(TryGetPawnOwner());

	if (OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UJujutsuCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!OwningCharacter || !OwningMovementComponent)
	{
		return;
	}

	GroundSpeed = OwningCharacter->GetVelocity().Size2D();

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D()>0.f;
}
