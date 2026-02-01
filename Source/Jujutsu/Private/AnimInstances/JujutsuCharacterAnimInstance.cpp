// Copyright Epic Games, Inc. All Rights Reserved.



#include "AnimInstances/JujutsuCharacterAnimInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "JujutsuGameplayTags.h"

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

	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	bIsFalling = OwningMovementComponent->IsFalling();
}

void UJujutsuCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	bIsHit = HasGameplayTag(JujutsuGameplayTags::Character_Status_Hit);
}

bool UJujutsuCharacterAnimInstance::HasGameplayTag(FGameplayTag Tag) const
{
	if (!OwningCharacter)
	{
		return false;
	}

	const IAbilitySystemInterface* ASI =
		Cast<IAbilitySystemInterface>(OwningCharacter);

	if (!ASI)
	{
		return false;
	}

	const UAbilitySystemComponent* ASC =
		ASI->GetAbilitySystemComponent();

	return ASC ? ASC->HasMatchingGameplayTag(Tag) : false;
}
