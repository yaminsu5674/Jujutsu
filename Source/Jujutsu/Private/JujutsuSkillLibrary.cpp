// Copyright Epic Games, Inc. All Rights Reserved.

#include "JujutsuSkillLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameplayEffect.h"
#include "JujutsuGameplayTags.h"
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

void UJujutsuSkillLibrary::SetObjectRotationToTarget(USceneComponent* Object, AActor* Target)
{
	if (!Object || !Target) return;

	FVector ObjectLocation = Object->GetComponentLocation();
	FVector TargetLocation = Target->GetActorLocation();
	FVector Direction = (TargetLocation - ObjectLocation).GetSafeNormal();

	FRotator LookAtRotation = Direction.Rotation();
	FRotator OnlyYawRotation = FRotator(0.f, LookAtRotation.Yaw + 90.0f, 0.f);
	Object->SetWorldRotation(OnlyYawRotation);
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

FGameplayEffectSpecHandle UJujutsuSkillLibrary::MakeDamageEffectSpecHandle(AActor* SourceActor, TSubclassOf<UGameplayEffect> EffectClass, float BaseDamage, int32 InUsedComboCount, int32 Level)
{
	if (!SourceActor || !EffectClass) return FGameplayEffectSpecHandle();

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!ASC) return FGameplayEffectSpecHandle();

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(SourceActor);
	ContextHandle.AddInstigator(SourceActor, SourceActor);

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	if (!EffectSpecHandle.IsValid()) return FGameplayEffectSpecHandle();

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		JujutsuGameplayTags::Character_SetByCaller_BaseDamage,
		BaseDamage
	);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		JujutsuGameplayTags::Character_SetByCaller_UsedComboCount,
		static_cast<float>(InUsedComboCount)
	);

	return EffectSpecHandle;
}

FActiveGameplayEffectHandle UJujutsuSkillLibrary::ApplyEffectSpecHandleToTarget(AActor* SourceActor, AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	if (!SourceActor || !TargetActor || !InSpecHandle.IsValid()) return FActiveGameplayEffectHandle();

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!SourceASC || !TargetASC) return FActiveGameplayEffectHandle();

	return SourceASC->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
}
