// Copyright Epic Games, Inc. All Rights Reserved.

#include "JujutsuSkillLibrary.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

void UJujutsuSkillLibrary::SetActorRotationToTarget(AJujutsuBaseCharacter* InCharacter)
{
	if (!InCharacter) return;

	UJujutsuCharacterCombatComponent* CombatComp = InCharacter->GetCharacterCombatComponent();
	if (!CombatComp) return;

	AJujutsuBaseCharacter* TargetChar = CombatComp->Target.Get();
	if (!TargetChar) return;

	const FVector CharLoc = InCharacter->GetActorLocation();
	const FVector TargetLoc = TargetChar->GetActorLocation();
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(CharLoc, TargetLoc);

	InCharacter->SetActorRotation(FRotator(0.f, LookAtRot.Yaw, 0.f));
}

FVector UJujutsuSkillLibrary::GetSpawnLocationFromCharacter(AActor* Actor, FVector Offset)
{
	if (!Actor) return FVector::ZeroVector;

	const FVector BaseLoc = Actor->GetActorLocation();
	const FVector Forward = Actor->GetActorForwardVector() * Offset.X;
	const FVector Right = Actor->GetActorRightVector() * Offset.Y;
	const FVector Up = Actor->GetActorUpVector() * Offset.Z;

	return BaseLoc + Forward + Right + Up;
}
