// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/Push/JujutsuPushComponent.h"

UJujutsuPushComponent::UJujutsuPushComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UJujutsuPushComponent::BeginPlay()
{
	Super::BeginPlay();
}
