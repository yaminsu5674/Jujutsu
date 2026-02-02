// Copyright Epic Games, Inc. All Rights Reserved.

#include "JujutsuSkillLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/JujutsuBaseCharacter.h"
#include "GameFramework/Character.h"
#include "Components/Combat/JujutsuCharacterCombatComponent.h"
#include "Components/JujutsuCharacterMovementComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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

void UJujutsuSkillLibrary::SetGravityEnabled(AJujutsuBaseCharacter* InCharacter, bool bEnable)
{
	if (!InCharacter) return;

	UCharacterMovementComponent* MoveComp = InCharacter->GetCharacterMovement();
	if (!MoveComp) return;

	if (bEnable)
	{
		if (const UJujutsuCharacterMovementComponent* JutsuMove = Cast<UJujutsuCharacterMovementComponent>(MoveComp))
		{
			MoveComp->GravityScale = JutsuMove->DefaultGravityScale;
		}
		else
		{
			MoveComp->GravityScale = 1.f;
		}
	}
	else
	{
		MoveComp->StopMovementImmediately();
		MoveComp->GravityScale = 0.f;
	}
}

void UJujutsuSkillLibrary::LaunchCharacterFromSourceToTarget(AActor* SourceActor, ACharacter* TargetCharacter, float HorizontalForce, float VerticalForce)
{
	if (!SourceActor || !TargetCharacter) return;

	FVector Dir2D = SourceActor->GetVelocity();
	Dir2D.Z = 0.f;
	if (Dir2D.IsNearlyZero())
	{
		Dir2D = SourceActor->GetActorForwardVector();
		Dir2D.Z = 0.f;
	}
	Dir2D = Dir2D.GetSafeNormal();

	FVector LaunchVelocity(Dir2D.X * HorizontalForce, Dir2D.Y * HorizontalForce, VerticalForce);
	TargetCharacter->LaunchCharacter(LaunchVelocity, true, true);
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

FActiveGameplayEffectHandle UJujutsuSkillLibrary::ApplyDamageEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass, float BaseDamage, int32 InUsedComboCount, int32 Level, AActor* InstigatorForContext)
{
	if (!TargetActor || !EffectClass) return FActiveGameplayEffectHandle();

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC) return FActiveGameplayEffectHandle();

	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
	if (InstigatorForContext)
	{
		ContextHandle.AddSourceObject(InstigatorForContext);
		ContextHandle.AddInstigator(InstigatorForContext, InstigatorForContext);
	}

	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	if (!EffectSpecHandle.IsValid()) return FActiveGameplayEffectHandle();

	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		JujutsuGameplayTags::Character_SetByCaller_BaseDamage,
		BaseDamage
	);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(
		JujutsuGameplayTags::Character_SetByCaller_UsedComboCount,
		static_cast<float>(InUsedComboCount)
	);

	return TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}
