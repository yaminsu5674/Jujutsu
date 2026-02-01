// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/JujutsuCharacterMovementComponent.h"

UJujutsuCharacterMovementComponent::UJujutsuCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxWalkSpeed = WalkSpeed;
	GravityScale = DefaultGravityScale;
}
